#include "tt-component-wood.h"

#include "tt-entities.h"
#include "tt-error.h"
#include "tt-storage-vector.h"


static struct TTComponentTargetState {
    bool initialised;
    TTStorageVector *storage;
} state = { .initialised = false };


void tt_component_wood_startup(void) {
    tt_assert(state.initialised == false);

    state.storage = tt_storage_vector_new(sizeof(TTEntityId));

    state.initialised = true;
}


void tt_component_wood_shutdown(void) {
    tt_assert(state.initialised == true);

    tt_storage_vector_free(state.storage);
    state.storage = NULL;

    state.initialised = false;
}


void tt_component_wood_set(
    TTEntityId entity_id, int wood
) {
    tt_assert(state.initialised == true);

    TTEntityId *storage = (TTEntityId *) tt_storage_vector_add(
        state.storage, entity_id
    );

    *storage = wood;
}


bool tt_component_wood_has(TTEntityId entity_id) {
    tt_assert(state.initialised == true);

    return tt_storage_vector_has(state.storage, entity_id);
}


int tt_component_wood_get(TTEntityId entity_id) {
    tt_assert(state.initialised == true);
    tt_assert(tt_storage_vector_has(state.storage, entity_id));

    int *storage = (int *) tt_storage_vector_get(
        state.storage, entity_id
    );

    return *storage;
}


void tt_component_wood_remove(TTEntityId entity_id) {
    tt_assert(state.initialised == true);
    tt_assert(tt_storage_vector_has(state.storage, entity_id));

    tt_storage_vector_remove(state.storage, entity_id);
}
