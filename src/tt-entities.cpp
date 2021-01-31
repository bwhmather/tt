#include "tt-entities.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <vector>

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
static bool maintaining = false;

static std::vector<bool> live_set;
static std::vector<bool> next_live_set;

static std::vector<TTEntityId> free_list;

int next_callback_handle = 1;
static std::vector<OnCreateCallbackState> on_create_callbacks;
static std::vector<OnDeleteCallbackState> on_delete_callbacks;


void tt_entities_startup(void) {
    assert(!initialized);
    assert(!maintaining);

    initialized = true;

    live_set.clear();
    live_set.push_back(true);
    next_live_set = std::vector<bool>(live_set);

    free_list.clear();

    on_create_callbacks.clear();
    on_delete_callbacks.clear();
}

void tt_entities_maintain(void) {
    assert(initialized);
    assert(!maintaining);

    maintaining = true;

    for (TTEntityId id = 1; id < live_set.size(); id++) {
        if (!live_set[id]) continue;
        if (next_live_set[id]) continue;

        for (const OnDeleteCallbackState& state : on_delete_callbacks) {
            state.callback(id, state.user_data);
        }
    }

    for (TTEntityId id = 1; id < next_live_set.size(); id++) {
        if (id < live_set.size() && live_set[id]) continue;
        if (!next_live_set[id]) continue;

        for (const OnCreateCallbackState& state : on_create_callbacks) {
            state.callback(id, state.user_data);
        }
    }

    free_list.clear();
    for (TTEntityId id = next_live_set.size() - 1; id > 0; id--) {
        if (!next_live_set[id]) {
            free_list.push_back(id);
        }
    }

    live_set = next_live_set;

    maintaining = false;
}

void tt_entities_shutdown(void) {
    assert(initialized);
    assert(!maintaining);

    live_set.clear();
    next_live_set.clear();
    free_list.clear();

    on_create_callbacks.clear();
    on_delete_callbacks.clear();

    initialized = false;
}

TTEntityId tt_entities_create(void) {
    TTEntityId entity_id;

    assert(initialized);
    assert(!maintaining);

    if (free_list.size()) {
        entity_id = free_list.back();
        free_list.pop_back();
        next_live_set[entity_id] = true;
    } else {
        entity_id = next_live_set.size();
        next_live_set.push_back(true);
    }

    return entity_id;
}

void tt_entities_delete(TTEntityId entity_id) {
    assert(initialized);
    assert(!maintaining);

    assert(next_live_set.at(entity_id) == true);

    next_live_set[entity_id] = false;

    for (const OnDeleteCallbackState& state : on_delete_callbacks) {
        state.callback(entity_id, state.user_data);
    }
}


int tt_entities_bind_on_create_callback(
    void (*callback) (TTEntityId id, void *user_data), void *user_data
) {
    assert(initialized);
    assert(!maintaining);

    OnCreateCallbackState& state = on_create_callbacks.emplace_back();

    state.handle = next_callback_handle++;
    state.callback = callback;
    state.user_data = user_data;

    return state.handle;
}

void tt_entities_unbind_on_create_callback(int handle) {
    assert(initialized);
    assert(!maintaining);

    std::remove_if(
        on_create_callbacks.begin(), on_create_callbacks.end(),
        [&handle](OnCreateCallbackState& state) {
            return state.handle == handle;
        }
    );
}

int tt_entities_bind_on_delete_callback(
    void (*callback) (TTEntityId id, void *user_data), void *user_data
) {
    assert(initialized);
    assert(!maintaining);

    OnDeleteCallbackState state = on_delete_callbacks.emplace_back();

    state.handle = next_callback_handle++;
    state.callback = callback;
    state.user_data = user_data;

    return state.handle;
}

void tt_entities_unbind_on_delete_callback(int handle) {
    assert(initialized);
    assert(!maintaining);

    std::remove_if(
        on_delete_callbacks.begin(), on_delete_callbacks.end(),
        [&handle](OnDeleteCallbackState& state) {
            return state.handle == handle;
        }
    );
}


void tt_entities_iter_begin(TTEntityIter *iter) {
    assert(initialized);
    assert(!maintaining);

    *iter = 1;
}

bool tt_entities_iter_has_next(TTEntityIter *iter) {
    assert(initialized);
    assert(!maintaining);

    return *iter < live_set.size();
}

TTEntityId tt_entities_iter_next(TTEntityIter *iter) {
    assert(initialized);
    assert(!maintaining);

    TTEntityId entity_id = (TTEntityId) *iter;

    assert(entity_id <= live_set.size());
    assert(live_set[entity_id] == true);

    while (true) {
        (*iter)++;

        if (*iter >= live_set.size()) break;
        if (live_set.at(*iter)) break;
    }

    return entity_id;
}
