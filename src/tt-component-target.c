#include "tt-component-target.h"

#include "tt-entities.h"
#include "tt-error.h"
#include "tt-storage-vector.h"

static struct TTComponentTargetState {
    bool initialised;
    TTStorageVector *storage;
} state = {.initialised = false};

void
tt_component_target_startup(void) {
    tt_assert(state.initialised == false);

    state.storage = tt_storage_vector_new(sizeof(TTEntityId));

    state.initialised = true;
}

void
tt_component_target_shutdown(void) {
    tt_assert(state.initialised == true);

    tt_storage_vector_free(state.storage);
    state.storage = NULL;

    state.initialised = false;
}

void
tt_component_target_set(TTEntityId entity_id, TTEntityId target_id) {
    tt_assert(state.initialised == true);

    TTEntityId *storage =
        (TTEntityId *)tt_storage_vector_add(state.storage, entity_id);

    *storage = target_id;
}

bool
tt_component_target_has(TTEntityId entity_id) {
    tt_assert(state.initialised == true);

    return tt_storage_vector_has(state.storage, entity_id);
}

TTEntityId
tt_component_target_get(TTEntityId entity_id) {
    tt_assert(state.initialised == true);
    tt_assert(tt_storage_vector_has(state.storage, entity_id));

    TTEntityId *storage =
        (TTEntityId *)tt_storage_vector_get(state.storage, entity_id);

    return *storage;
}

void
tt_component_target_remove(TTEntityId entity_id) {
    tt_assert(state.initialised == true);
    tt_assert(tt_storage_vector_has(state.storage, entity_id));

    tt_storage_vector_remove(state.storage, entity_id);
}
