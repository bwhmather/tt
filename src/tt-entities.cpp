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
#include "tt-vector.h"
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
    static TTVector on_create_callbacks;
    static TTVector on_delete_callbacks;
}


extern "C" void tt_entities_startup(void) {
    tt_assert(state::initialised == false);
    tt_assert(state::maintaining == false);

    state::initialised = true;

    tt_bitset_init(&state::live_set);
    tt_bitset_init(&state::next_live_set);

    state::max_entity_id = 0;
    state::next_free_entity_id = 1;

    tt_vector_init(&state::on_create_callbacks, sizeof(OnCreateCallbackState));
    tt_vector_init(&state::on_delete_callbacks, sizeof(OnDeleteCallbackState));
}

extern "C" void tt_entities_maintain(void) {
    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    state::maintaining = true;

    for (TTEntityId id = 1; id <= state::max_entity_id; id++) {
        if (!tt_bitset_get(&state::live_set, id)) continue;
        if (tt_bitset_get(&state::next_live_set, id)) continue;

        for (
            size_t i = 0;
            i < tt_vector_item_count(&state::on_delete_callbacks);
            i++
        ) {
            OnDeleteCallbackState *cb_state = (OnDeleteCallbackState *)
                tt_vector_get(&state::on_delete_callbacks, i);

            cb_state->callback(id, cb_state->user_data);
        }
    }

    for (TTEntityId id = 1; id <= state::max_entity_id; id++) {
        if (tt_bitset_get(&state::live_set, id)) continue;
        if (!tt_bitset_get(&state::next_live_set, id)) continue;

        for (
            size_t i = 0;
            i < tt_vector_item_count(&state::on_create_callbacks);
            i++
        ) {
            OnCreateCallbackState *cb_state = (OnCreateCallbackState *)
                tt_vector_get(&state::on_create_callbacks, i);

            cb_state->callback(id, cb_state->user_data);
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

    tt_bitset_destroy(&state::live_set);
    tt_bitset_destroy(&state::next_live_set);

    state::max_entity_id = 0;
    state::next_free_entity_id = 1;

    tt_vector_destroy(&state::on_create_callbacks);
    tt_vector_destroy(&state::on_delete_callbacks);

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

    OnCreateCallbackState *cb_state = (OnCreateCallbackState *)
        tt_vector_push(&state::on_create_callbacks);

    cb_state->handle = state::next_callback_handle++;
    cb_state->callback = callback;
    cb_state->user_data = user_data;

    return cb_state->handle;
}

extern "C" void tt_entities_unbind_on_create_callback(int handle) {
    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    for (
        size_t i = 0;
        i < tt_vector_item_count(&state::on_create_callbacks);
        i++
    ) {
        OnCreateCallbackState *cb_state = (OnCreateCallbackState *)
            tt_vector_get(&state::on_create_callbacks, i);
        if (cb_state->handle == handle) {
            tt_vector_remove(&state::on_create_callbacks, i);
            return;
        }
    }

    tt_assert(false);  // No callback found.
}


extern "C" int tt_entities_bind_on_delete_callback(
    void (*callback) (TTEntityId id, void *user_data), void *user_data
) {
    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    OnDeleteCallbackState *cb_state = (OnDeleteCallbackState *)
        tt_vector_push(&state::on_delete_callbacks);

    cb_state->handle = state::next_callback_handle++;
    cb_state->callback = callback;
    cb_state->user_data = user_data;

    return cb_state->handle;
}

extern "C" void tt_entities_unbind_on_delete_callback(int handle) {
    tt_assert(state::initialised == true);
    tt_assert(state::maintaining == false);

    for (
        size_t i = 0;
        i < tt_vector_item_count(&state::on_delete_callbacks);
        i++
    ) {
        OnDeleteCallbackState *cb_state = (OnDeleteCallbackState *)
            tt_vector_get(&state::on_delete_callbacks, i);
        if (cb_state->handle == handle) {
            tt_vector_remove(&state::on_delete_callbacks, i);
            return;
        }
    }

    tt_assert(false);  // No callback found.
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
