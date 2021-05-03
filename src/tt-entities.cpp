#include "tt-entities.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <vector>

namespace tt {
namespace entities {

typedef struct OnCreateCallbackState {
    int handle;
    void (*callback) (tt::EntityId id, void *user_data);
    void *user_data;
} OnCreateCallbackState;

typedef struct OnDeleteCallbackState {
    int handle;
    void (*callback) (tt::EntityId id, void *user_data);
    void *user_data;
} OnDeleteCallbackState;

namespace state {
    static bool initialised = false;
    static bool maintaining = false;

    static std::vector<bool> live_set;
    static std::vector<bool> next_live_set;

    static std::vector<tt::EntityId> free_list;

    int next_callback_handle = 1;
    static std::vector<OnCreateCallbackState> on_create_callbacks;
    static std::vector<OnDeleteCallbackState> on_delete_callbacks;
}

void startup(void) {
    assert(!state::initialised);
    assert(!state::maintaining);

    state::initialised = true;

    state::live_set.clear();
    state::live_set.push_back(true);
    state::next_live_set = std::vector<bool>(state::live_set);

    state::free_list.clear();

    state::on_create_callbacks.clear();
    state::on_delete_callbacks.clear();
}

void maintain(void) {
    assert(state::initialised);
    assert(!state::maintaining);

    state::maintaining = true;

    for (tt::EntityId id = 1; id < state::live_set.size(); id++) {
        if (!state::live_set[id]) continue;
        if (state::next_live_set[id]) continue;

        for (
            const OnDeleteCallbackState& cb_state :
            state::on_delete_callbacks
        ) {
            cb_state.callback(id, cb_state.user_data);
        }
    }

    for (tt::EntityId id = 1; id < state::next_live_set.size(); id++) {
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
    for (tt::EntityId id = state::next_live_set.size() - 1; id > 0; id--) {
        if (!state::next_live_set[id]) {
            state::free_list.push_back(id);
        }
    }

    state::live_set = state::next_live_set;

    state::maintaining = false;
}

void shutdown(void) {
    assert(state::initialised);
    assert(!state::maintaining);

    state::live_set.clear();
    state::next_live_set.clear();
    state::free_list.clear();

    state::on_create_callbacks.clear();
    state::on_delete_callbacks.clear();

    state::initialised = false;
}

tt::EntityId create(void) {
    tt::EntityId entity_id;

    assert(state::initialised);
    assert(!state::maintaining);

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

void remove(tt::EntityId entity_id) {
    assert(state::initialised);
    assert(!state::maintaining);

    assert(state::next_live_set.at(entity_id) == true);

    state::next_live_set[entity_id] = false;

    for (const OnDeleteCallbackState& cb_state : state::on_delete_callbacks) {
        cb_state.callback(entity_id, cb_state.user_data);
    }
}


int bind_on_create_callback(
    void (*callback) (tt::EntityId id, void *user_data), void *user_data
) {
    assert(state::initialised);
    assert(!state::maintaining);

    OnCreateCallbackState& cb_state =
        state::on_create_callbacks.emplace_back();

    cb_state.handle = state::next_callback_handle++;
    cb_state.callback = callback;
    cb_state.user_data = user_data;

    return cb_state.handle;
}

void unbind_on_create_callback(int handle) {
    assert(state::initialised);
    assert(!state::maintaining);

    std::remove_if(
        state::on_create_callbacks.begin(),
        state::on_create_callbacks.end(),
        [&handle](OnCreateCallbackState& cb_state) {
            return cb_state.handle == handle;
        }
    );
}

int bind_on_delete_callback(
    void (*callback) (tt::EntityId id, void *user_data), void *user_data
) {
    assert(state::initialised);
    assert(!state::maintaining);

    OnDeleteCallbackState cb_state = state::on_delete_callbacks.emplace_back();

    cb_state.handle = state::next_callback_handle++;
    cb_state.callback = callback;
    cb_state.user_data = user_data;

    return cb_state.handle;
}

void unbind_on_delete_callback(int handle) {
    assert(state::initialised);
    assert(!state::maintaining);

    std::remove_if(
        state::on_delete_callbacks.begin(),
        state::on_delete_callbacks.end(),
        [&handle](OnDeleteCallbackState& cb_state) {
            return cb_state.handle == handle;
        }
    );
}

void iter_begin(tt::EntityIter *iter) {
    assert(state::initialised);
    assert(!state::maintaining);

    *iter = 1;
}

bool iter_has_next(tt::EntityIter *iter) {
    assert(state::initialised);
    assert(!state::maintaining);

    return *iter < state::live_set.size();
}

tt::EntityId iter_next(tt::EntityIter *iter) {
    assert(state::initialised);
    assert(!state::maintaining);

    tt::EntityId entity_id = (tt::EntityId) *iter;

    assert(entity_id <= state::live_set.size());
    assert(state::live_set[entity_id] == true);

    while (true) {
        (*iter)++;

        if (*iter >= state::live_set.size()) break;
        if (state::live_set.at(*iter)) break;
    }

    return entity_id;
}

}  /* namespace entities */
}  /* namespace tt */
