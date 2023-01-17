#include "tt-component-position.h"

#include "tt-entities.h"
#include "tt-error.h"
#include "tt-storage-vector.h"

static struct TTComponentPositionState {
    bool initialised;
    TTStorageVector *storage;
} state = {.initialised = false};

void
tt_component_position_startup(void) {
    tt_assert(state.initialised == false);

    state.storage = tt_storage_vector_new(sizeof(TTPosition));

    state.initialised = true;
}

void
tt_component_position_shutdown(void) {
    tt_assert(state.initialised == true);

    tt_storage_vector_free(state.storage);
    state.storage = NULL;

    state.initialised = false;
}

TTPosition *
tt_component_position_add(TTEntityId entity_id) {
    tt_assert(state.initialised == true);
    tt_assert(!tt_storage_vector_has(state.storage, entity_id));

    TTPosition *position =
        (TTPosition *)tt_storage_vector_add(state.storage, entity_id);

    *position = (TTPosition){.x = 0, .y = 0};

    return position;
}

bool
tt_component_position_has(TTEntityId entity_id) {
    tt_assert(state.initialised == true);

    return tt_storage_vector_has(state.storage, entity_id);
}

TTPosition *
tt_component_position_get(TTEntityId entity_id) {
    tt_assert(state.initialised == true);
    tt_assert(tt_storage_vector_has(state.storage, entity_id));

    return (TTPosition *)tt_storage_vector_get(state.storage, entity_id);
}

void
tt_component_position_remove(TTEntityId entity_id) {
    tt_assert(state.initialised == true);
    tt_assert(tt_storage_vector_has(state.storage, entity_id));

    tt_storage_vector_remove(state.storage, entity_id);
}
