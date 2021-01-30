#include "tt-entities.hpp"

#include <cassert>
#include <cstdint>
#include <cstdlib>

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

static bool initialized = false;

static std::vector<bool> this_live_set;
static std::vector<bool> next_live_set;

static std::vector<TTEntityId> free_list;

int next_callback_handle = 1;
static std::vector<OnCreateCallbackState> on_create_callbacks;
static std::vector<OnDeleteCallbackState> on_delete_callbacks;

void tt_entities_startup(void) {
    assert(!initialized);
    initialized = true;
}

void tt_entities_maintain(void) {
    static std::vector<bool> prev_live_set(next_live_set);
    std::swap(prev_live_set, this_live_set);

}


void tt_entities_shutdown(void) {
    assert(initialized);

    live_set.clear();
    free_list.clear();

    on_create_callbacks.clear();
    on_delete_callbacks.clear();

    initialized = false;
}

TTEntityId tt_entities_create(void) {
    TTEntityId entity_id;

    assert(initialized);

    if (free_list.size()) {
        entity_id = free_list.pop_back();
        live_set[entity_id] = true;
    } else {
        entity_id = live_set.size();
        live_set.push_back(true);
    }

    for (const &OnCreateCallbackState state : on_create_callbacks) {
        state.callback(entity_id, state.user_data);
    }

    return entity_id;
}

void tt_entities_delete(TTEntityId entity_id) {
    assert(initialized);

    assert(live_set.at(entity_id) == true);

    live_set[entity_id] = false;

    for (const &OnDeleteCallbackState state : on_delete_callbacks) {
        state.callback(entity_id, state.user_data);
    }
}


int tt_entities_bind_on_create_callback(
    void (*callback) (TTEntityId id, void *user_data), void *user_data
) {
    assert(initialized);

    &OnCreateCallback state = on_create_callbacks.emplace_back();

    state.handle = next_callback_handle++;
    state.callback = callback;
    state.user_data = user_data;

    return state.handle;
}

void tt_entities_unbind_create_callback(int handle) {
    assert(initialised);
    // TODO;
}

int tt_entities_bind_on_delete_callback(
    void (*callback) (TTEntityId id, void *user_data), void *user_data
) {
    assert(initialized);

    &OnDeleteCallback state = on_delete_callbacks.emplace_back();

    state.handle = next_callback_handle++;
    state.callback = callback;
    state.user_data = user_data;

    return state.handle;
}

void tt_entities_unbind_delete_callback(int handle) {
    assert(initialised);
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

    assert(entity_id <= live_set.size());
    assert(live_set[entity_id] == true);

    while (true) {
        (*iter)++;

        if (*iter > tt_entities_max) break;
        if (tt_storage_bitset_contains(tt_entities_live_set, *iter)) break;
    }

    return entity_id;
}
