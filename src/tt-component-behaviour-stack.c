#include "tt-component-behaviour-stack.h"

#include <stddef.h>

#include "bt.h"
#include "tt-behaviour.h"
#include "tt-error.h"
#include "tt-storage-vector.h"

const size_t STACK_SIZE = 128;

static struct TTComponentBehaviourStackState {
    bool initialised;
    TTStorageVector *storage;
} state = {.initialised = false};

void
tt_component_behaviour_stack_startup(void) {
    tt_assert(state.initialised == false);

    state.storage = tt_storage_vector_new(STACK_SIZE);

    state.initialised = true;
}

void
tt_component_behaviour_stack_shutdown(void) {
    tt_assert(state.initialised == true);

    tt_storage_vector_free(state.storage);
    state.storage = NULL;

    state.initialised = false;
}

void
tt_component_behaviour_stack_init(TTEntityId entity_id) {
    tt_assert(state.initialised == true);
    tt_assert(!tt_storage_vector_has(state.storage, entity_id));

    BTStack *stack = (BTStack *)tt_storage_vector_add(state.storage, entity_id);
    bt_stack_init(stack, STACK_SIZE);
}

bool
tt_component_behaviour_stack_has(TTEntityId entity_id) {
    tt_assert(state.initialised == true);

    return tt_storage_vector_has(state.storage, entity_id);
}

BTStack *
tt_component_behaviour_stack_get(TTEntityId entity_id) {
    tt_assert(state.initialised == true);
    tt_assert(tt_storage_vector_has(state.storage, entity_id));

    return (BTStack *)tt_storage_vector_get(state.storage, entity_id);
}

void
tt_component_behaviour_stack_remove(TTEntityId entity_id) {
    tt_assert(state.initialised == true);
    tt_assert(tt_storage_vector_has(state.storage, entity_id));

    // TODO we also need to do this when we shutdown the component.
    BTStack *stack = (BTStack *)tt_storage_vector_get(state.storage, entity_id);
    bt_stack_shutdown(stack, STACK_SIZE);

    tt_storage_vector_remove(state.storage, entity_id);
}
