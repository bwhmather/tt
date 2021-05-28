extern "C" {
#include "tt-component-behaviour.h"
}

#include <cstddef>

#include "tt-storage-sparse-vector.tpp"

extern "C" {
#include "tt-behaviour.h"
#include "tt-error.h"
}

const std::size_t MAX_STACK = 128;

struct TTComponentBehaviour {
    TTBehaviour *prev;
    TTBehaviour *next;
    char stack[MAX_STACK];
};


namespace state {
    static bool initialised = false;
    static TTStorageSparseVector<TTComponentBehaviour> *storage = NULL;
}

extern "C" void tt_component_behaviour_startup(void) {
    tt_assert(state::initialised == false);

    state::storage = new TTStorageSparseVector<TTComponentBehaviour>();

    state::initialised = true;
}

extern "C" void tt_component_behaviour_shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;

    state::initialised = false;
}

extern "C" void tt_component_behaviour_set_prev(
    TTEntityId entity_id, TTBehaviour *behaviour
) {
    tt_assert(state::initialised == true);
    tt_assert(tt_behaviour_max_stack_size(behaviour) < MAX_STACK);

    if (!state::storage->has(entity_id)) {
        state::storage->add(entity_id, { NULL, NULL, { 0 }});
    }

    TTComponentBehaviour &component = state::storage->get(entity_id);
    component.prev = behaviour;
}

extern "C" void tt_component_behaviour_set_next(
    TTEntityId entity_id, TTBehaviour *behaviour
) {
    tt_assert(state::initialised == true);
    tt_assert(tt_behaviour_max_stack_size(behaviour) < MAX_STACK);

    if (!state::storage->has(entity_id)) {
        state::storage->add(entity_id, { NULL, NULL, { 0 }});
    }

    TTComponentBehaviour &component = state::storage->get(entity_id);
    component.next = behaviour;
}

extern "C" bool tt_component_behaviour_has(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->has(entity_id);
}

extern "C" TTBehaviour *tt_component_behaviour_get_prev(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->get(entity_id).prev;
}

extern "C" TTBehaviour *tt_component_behaviour_get_next(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->get(entity_id).next;
}

extern "C" void *tt_component_behaviour_get_stack(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    // Note that we return a pointer to the _end_ of the stack, not the
    // beginning.
    return (void *) &state::storage->get(entity_id).stack[MAX_STACK];
}

extern "C" void tt_component_behaviour_remove(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->remove(entity_id);
}
