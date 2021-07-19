extern "C" {
#include "tt-entities.h"
}

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <vector>

extern "C" {
#include "tt-error.h"
}

typedef struct OnCreateCallbackState {
    int handle;
    void (*callback) (TTEntityId id, void *user_data);
    void *user_data;
} OnCreateCallbackState;

typedef struct OnDeleteCallbackState {
    int handle;
    void (*callback) (TTEntityId id, void *user_data);
    void *user_data;
} OnDeleteCallbackState;

namespace state {
    static bool initialised = false;
    static bool maintaining = false;

    static std::vector<bool> live_set;
    static std::vector<bool> next_live_set;

    static std::vector<TTEntityId> free_list;

    int next_callback_handle = 1;
    static std::vector<OnCreateCallbackState> on_create_callbacks;
    static std::vector<OnDeleteCallbackState> on_delete_callbacks;
}

extern "C" void tt_entities_startup(void) {
    tt_assert(state::initialised == false);
    tt_assert(state::maintaining == false);

    state::initialised = true;

    state::live_set.clear();
    state::live_set.push_back(true);
    state::next_live_set = std::vector<bool>(state::live_set);

    state::free_list.clear();

    state::on_create_callbacks.clear();
    state::on_delete_callbacks.clear();
}

extern "C" void tt_entities_maintain(void) {
    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    state::maintaining = true;

    for (TTEntityId id = 1; id < state::live_set.size(); id++) {
        if (!state::live_set[id]) continue;
        if (state::next_live_set[id]) continue;

        for (
            const OnDeleteCallbackState& cb_state :
            state::on_delete_callbacks
        ) {
            cb_state.callback(id, cb_state.user_data);
        }
    }

    for (TTEntityId id = 1; id < state::next_live_set.size(); id++) {
        if (id < state::live_set.size() && state::live_set[id]) continue;
        if (!state::next_live_set[id]) continue;

        for (
            const OnCreateCallbackState& cb_state :
            state::on_create_callbacks
        ) {
            cb_state.callback(id, cb_state.user_data);
        }
    }

    state::free_list.clear();
    for (TTEntityId id = state::next_live_set.size() - 1; id > 0; id--) {
        if (!state::next_live_set[id]) {
            state::free_list.push_back(id);
        }
    }

    state::live_set = state::next_live_set;

    state::maintaining = false;
}

extern "C" void tt_entities_shutdown(void) {
    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    state::live_set.clear();
    state::next_live_set.clear();
    state::free_list.clear();

    state::on_create_callbacks.clear();
    state::on_delete_callbacks.clear();

    state::initialised = false;
}

extern "C" TTEntityId tt_entities_create(void) {
    TTEntityId entity_id;

    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    if (state::free_list.size()) {
        entity_id = state::free_list.back();
        state::free_list.pop_back();
        state::next_live_set[entity_id] = true;
    } else {
        entity_id = state::next_live_set.size();
        state::next_live_set.push_back(true);
    }

    return entity_id;
}

extern "C" void tt_entities_remove(TTEntityId entity_id) {
    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    tt_assert(state::next_live_set.at(entity_id) == true);

    state::next_live_set[entity_id] = false;

    for (const OnDeleteCallbackState& cb_state : state::on_delete_callbacks) {
        cb_state.callback(entity_id, cb_state.user_data);
    }
}


extern "C" int tt_entities_bind_on_create_callback(
    void (*callback) (TTEntityId id, void *user_data), void *user_data
) {
    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    OnCreateCallbackState& cb_state =
        state::on_create_callbacks.emplace_back();

    cb_state.handle = state::next_callback_handle++;
    cb_state.callback = callback;
    cb_state.user_data = user_data;

    return cb_state.handle;
}

extern "C" void tt_entities_unbind_on_create_callback(int handle) {
    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    auto callback = std::find_if(
        state::on_create_callbacks.begin(),
        state::on_create_callbacks.end(),
        [&handle](OnCreateCallbackState& cb_state) {
            return cb_state.handle == handle;
        }
    );

    tt_assert(callback != state::on_create_callbacks.end());

    state::on_create_callbacks.erase(callback);
}

extern "C" int tt_entities_bind_on_delete_callback(
    void (*callback) (TTEntityId id, void *user_data), void *user_data
) {
    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    OnDeleteCallbackState& cb_state =
        state::on_delete_callbacks.emplace_back();

    cb_state.handle = state::next_callback_handle++;
    cb_state.callback = callback;
    cb_state.user_data = user_data;

    return cb_state.handle;
}

extern "C" void tt_entities_unbind_on_delete_callback(int handle) {
    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    auto callback = std::find_if(
        state::on_delete_callbacks.begin(),
        state::on_delete_callbacks.end(),
        [&handle](OnDeleteCallbackState& cb_state) {
            return cb_state.handle == handle;
        }
    );

    tt_assert(callback != state::on_delete_callbacks.end());

    state::on_delete_callbacks.erase(callback);
}

extern "C" void tt_entities_iter_begin(TTEntityIter *iter) {
    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    *iter = 1;
}

extern "C" bool tt_entities_iter_has_next(TTEntityIter *iter) {
    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    return *iter < state::live_set.size();
}

extern "C" TTEntityId tt_entities_iter_next(TTEntityIter *iter) {
    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    TTEntityId entity_id = (TTEntityId) *iter;

    tt_assert(entity_id <= state::live_set.size());
    tt_assert(state::live_set[entity_id] == true);

    while (true) {
        (*iter)++;

        if (*iter >= state::live_set.size()) break;
        if (state::live_set.at(*iter)) break;
    }

    return entity_id;
}
