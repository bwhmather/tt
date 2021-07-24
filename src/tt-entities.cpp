extern "C" {
#include "tt-entities.h"
}

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

extern "C" {
#include "tt-bitset.h"
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

    static TTBitset live_set;
    static TTBitset next_live_set;

    static TTEntityId max_entity_id;
    static TTEntityId next_free_entity_id;

    int next_callback_handle = 1;
    static std::vector<OnCreateCallbackState> on_create_callbacks;
    static std::vector<OnDeleteCallbackState> on_delete_callbacks;
}


extern "C" void tt_entities_startup(void) {
    tt_assert(state::initialised == false);
    tt_assert(state::maintaining == false);

    state::initialised = true;

    tt_bitset_init(&state::live_set);
    tt_bitset_init(&state::next_live_set);

    state::max_entity_id = 0;
    state::next_free_entity_id = 1;

    state::on_create_callbacks.clear();
    state::on_delete_callbacks.clear();
}

extern "C" void tt_entities_maintain(void) {
    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    state::maintaining = true;

    for (TTEntityId id = 1; id <= state::max_entity_id; id++) {
        if (!tt_bitset_get(&state::live_set, id)) continue;
        if (tt_bitset_get(&state::next_live_set, id)) continue;

        for (
            const OnDeleteCallbackState& cb_state :
            state::on_delete_callbacks
        ) {
            cb_state.callback(id, cb_state.user_data);
        }
    }

    for (TTEntityId id = 1; id <= state::max_entity_id; id++) {
        if (tt_bitset_get(&state::live_set, id)) continue;
        if (!tt_bitset_get(&state::next_live_set, id)) continue;

        for (
            const OnCreateCallbackState& cb_state :
            state::on_create_callbacks
        ) {
            cb_state.callback(id, cb_state.user_data);
        }
    }

    state::next_free_entity_id = 1;
    while (tt_bitset_get(&state::next_live_set, state::next_free_entity_id)) {
        state::next_free_entity_id++;
    }
    tt_assert(state::next_free_entity_id <= state::max_entity_id + 1);

    tt_bitset_copy(&state::next_live_set, &state::live_set);

    state::maintaining = false;
}

extern "C" void tt_entities_shutdown(void) {
    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    tt_bitset_clear(&state::live_set);
    tt_bitset_clear(&state::next_live_set);

    state::max_entity_id = 0;
    state::next_free_entity_id = 1;

    state::on_create_callbacks.clear();
    state::on_delete_callbacks.clear();

    state::initialised = false;
}

extern "C" TTEntityId tt_entities_create(void) {
    TTEntityId entity_id;

    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    entity_id = state::next_free_entity_id;
    tt_bitset_set(&state::next_live_set, entity_id);

    if (entity_id > state::max_entity_id) {
        state::max_entity_id = entity_id;
    }

    while (true) {
        state::next_free_entity_id++;

        if (tt_bitset_get(
            &state::live_set, state::next_free_entity_id
        )) continue;

        if (tt_bitset_get(
            &state::next_live_set, state::next_free_entity_id
        )) continue;

        break;
    };

    tt_assert(state::next_free_entity_id <= state::max_entity_id + 1);

    return entity_id;
}

extern "C" void tt_entities_remove(TTEntityId entity_id) {
    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    tt_assert(tt_bitset_get(&state::next_live_set, entity_id));

    // With some work, it might be possible to relax this restriction, but with
    // the current implementation this will result in on-create and on-delete
    // callbacks being called.  Fix if it turns out to be a problem.
    tt_assert(tt_bitset_get(&state::live_set, entity_id));

    tt_bitset_unset(&state::next_live_set, entity_id);
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

    *iter = 0;

    while (true) {
        (*iter)++;

        if (*iter > state::max_entity_id) break;
        if (tt_bitset_get(&state::live_set, *iter)) break;
    }
}

extern "C" bool tt_entities_iter_has_next(TTEntityIter *iter) {
    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    return *iter <= state::max_entity_id;
}

extern "C" TTEntityId tt_entities_iter_next(TTEntityIter *iter) {
    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    TTEntityId entity_id = (TTEntityId) *iter;

    tt_assert(entity_id <= state::max_entity_id);
    tt_assert(tt_bitset_get(&state::live_set, entity_id));

    while (true) {
        (*iter)++;

        if (*iter > state::max_entity_id) break;
        if (tt_bitset_get(&state::live_set, *iter)) break;
    }

    return entity_id;
}
