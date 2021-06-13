#include "tt-component-behaviour.h"

#include "bt.h"
#include "tt-behaviour.h"
#include "tt-error.h"
#include "tt-storage-vector.h"


static struct TTSystemBehaviourState {
    bool initialised;
    TTStorageVector *storage;
} state = { .initialised = false };


void tt_component_behaviour_startup(void) {
    tt_assert(state.initialised == false);

    state.storage = tt_storage_vector_new(sizeof(BTBehaviour *));

    state.initialised = true;
}


void tt_component_behaviour_shutdown(void) {
    tt_assert(state.initialised == true);

    tt_storage_vector_free(state.storage);
    state.storage = NULL;

    state.initialised = false;
}


void tt_component_behaviour_set(
    TTEntityId entity_id, BTBehaviour *behaviour
) {
    tt_assert(state.initialised == true);

    BTBehaviour **storage = (BTBehaviour **) tt_storage_vector_add(
        state.storage, entity_id
    );

    *storage = behaviour;
}


bool tt_component_behaviour_has(TTEntityId entity_id) {
    tt_assert(state.initialised == true);

    return tt_storage_vector_has(state.storage, entity_id);
}


BTBehaviour *tt_component_behaviour_get(TTEntityId entity_id) {
    tt_assert(state.initialised == true);
    tt_assert(tt_storage_vector_has(state.storage, entity_id));

    BTBehaviour **storage = (BTBehaviour **) tt_storage_vector_get(
        state.storage, entity_id
    );

    return *storage;
}


void tt_component_behaviour_remove(TTEntityId entity_id) {
    tt_assert(state.initialised == true);
    tt_assert(tt_storage_vector_has(state.storage, entity_id));

    tt_storage_vector_remove(state.storage, entity_id);
}
