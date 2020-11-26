#include "tt-entities.hpp"

#include "tt-storage-bitset.hpp"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

typedef struct TTReleaseCallbackState {
    int handle;
    void (*callback) (TTEntityId id);
} TTReleaseCallbackState;


static bool tt_entities_is_initialised;
static size_t tt_entities_max = 0;

static TTStorageBitSet *tt_entities_live_set = NULL;

static size_t tt_entities_free_list_length = 0;
static size_t tt_entities_free_list_capacity = 0;
static TTEntityId *tt_entities_free_list = NULL;

static int tt_entities_release_callback_next_handle = 1;
static size_t tt_entities_release_callback_list_length = 0;
static size_t tt_entities_release_callback_list_capacity = 0;
static TTReleaseCallbackState *tt_entities_release_callback_list = NULL;


void tt_entities_startup(void) {
    assert(!tt_entities_is_initialised);

    tt_entities_live_set = tt_storage_bitset_new();

    tt_entities_free_list_length = 0;
    tt_entities_free_list_capacity = 256;
    tt_entities_free_list = (TTEntityId *) calloc(
        sizeof(TTEntityId), tt_entities_free_list_capacity
    );
    assert(tt_entities_free_list != NULL);

    tt_entities_release_callback_list_length = 0;
    tt_entities_release_callback_list_capacity = 256;
    tt_entities_release_callback_list = (TTReleaseCallbackState *) calloc(
        sizeof(TTReleaseCallbackState),
        tt_entities_release_callback_list_capacity
    );
    assert(tt_entities_release_callback_list != NULL);

    tt_entities_is_initialised = true;
}

void tt_entities_shutdown(void) {
    assert(tt_entities_is_initialised);

    tt_storage_bitset_free(tt_entities_live_set);
    tt_entities_live_set = NULL;

    tt_entities_free_list_length = 0;
    tt_entities_free_list_capacity = 0;
    free(tt_entities_free_list);
    tt_entities_free_list = NULL;

    tt_entities_release_callback_list_length = 0;
    tt_entities_release_callback_list_capacity = 0;
    free(tt_entities_release_callback_list);
    tt_entities_release_callback_list = NULL;

    tt_entities_is_initialised = false;
}

TTEntityId tt_entities_new_id(void) {
    TTEntityId entity_id;

    assert(tt_entities_is_initialised);

    if (tt_entities_free_list_length) {
        tt_entities_free_list_length--;
        entity_id = tt_entities_free_list[tt_entities_free_list_length];
    } else {
        tt_entities_max++;
        entity_id = tt_entities_max;
    }

    assert(!tt_storage_bitset_contains(tt_entities_live_set, entity_id));
    tt_storage_bitset_add(tt_entities_live_set, entity_id);

    return entity_id;
}

void tt_entities_release_id(TTEntityId entity_id) {
    assert(tt_entities_free_list != NULL);

    assert(entity_id != 0);
    assert(entity_id <= tt_entities_max);
    assert(tt_storage_bitset_contains(tt_entities_live_set, entity_id));

    for (size_t i = 0; i < tt_entities_release_callback_list_length; i++) {
        TTReleaseCallbackState *state = &tt_entities_release_callback_list[i];
        state->callback(entity_id);
    }

    if (tt_entities_free_list_length == tt_entities_free_list_capacity) {
        tt_entities_free_list_capacity += tt_entities_free_list_capacity / 2;
        tt_entities_free_list = realloc(
            tt_entities_free_list,
            tt_entities_free_list_capacity * sizeof(TTEntityId)
        );
        assert(tt_entities_free_list != NULL);
    }

    tt_entities_free_list[tt_entities_free_list_length] = entity_id;
    tt_entities_free_list_length++;

    tt_storage_bitset_remove(tt_entities_live_set, entity_id);
}

int tt_entities_bind_release_callback(
    void (*callback) (TTEntityId id)
) {
    assert(tt_entities_is_initialised);
    // TODO: Resize array.
    assert(
        tt_entities_release_callback_list_length <
        tt_entities_release_callback_list_capacity
    );

    int handle = tt_entities_release_callback_next_handle;
    tt_entities_release_callback_next_handle++;

    TTReleaseCallbackState *state = &tt_entities_release_callback_list[
        tt_entities_release_callback_list_length
    ];

    state->handle = handle;
    state->callback = callback;

    return handle;
}

void tt_entities_unbind_release_callback(int handle) {
    assert(tt_entities_is_initialised);
    // TODO;
}


void tt_entity_iter_begin(TTEntityIter *iter) {
    *iter = 1;
}

bool tt_entity_iter_has_next(TTEntityIter *iter) {
    return *iter <= tt_entities_max ? true : false;
}

TTEntityId tt_entity_iter_next(TTEntityIter *iter) {
    TTEntityId entity_id = (TTEntityId) *iter;

    assert(entity_id <= tt_entities_max);
    assert(tt_storage_bitset_contains(tt_entities_live_set, entity_id));

    while (true) {
        (*iter)++;

        if (*iter > tt_entities_max) break;
        if (tt_storage_bitset_contains(tt_entities_live_set, *iter)) break;
    }

    return entity_id;
}
