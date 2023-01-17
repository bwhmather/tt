#include "tt-component-harvestable.h"

#include "tt-entities.h"
#include "tt-error.h"
#include "tt-storage-bitset.h"

static struct TTComponentHarvestableState {
    bool initialised;
    TTStorageBitset *storage;
} state = {.initialised = false};

void
tt_component_harvestable_startup(void) {
    tt_assert(state.initialised == false);

    state.storage = tt_storage_bitset_new();

    state.initialised = true;
}

void
tt_component_harvestable_shutdown(void) {
    tt_assert(state.initialised == true);

    tt_storage_bitset_free(state.storage);
    state.storage = NULL;

    state.initialised = false;
}

void
tt_component_harvestable_set(TTEntityId entity_id, bool harvestable) {
    tt_assert(state.initialised == true);

    if (harvestable && !tt_storage_bitset_contains(state.storage, entity_id)) {
        tt_storage_bitset_add(state.storage, entity_id);
    }

    if (!harvestable && tt_storage_bitset_contains(state.storage, entity_id)) {
        tt_storage_bitset_remove(state.storage, entity_id);
    }
}

bool
tt_component_harvestable_get(TTEntityId entity_id) {
    tt_assert(state.initialised == true);

    return tt_storage_bitset_contains(state.storage, entity_id);
}
