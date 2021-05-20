#include "tt-component-harvestable.hpp"

#include <assert.h>

#include "tt-storage-bitset.tpp"


static TTStorageBitset *storage = NULL;

void tt_component_harvestable_startup(void) {
    assert(storage == NULL);
    storage = new TTStorageBitset();
}

void tt_component_harvestable_shutdown(void) {
    assert(storage != NULL);
    delete storage;
    storage = NULL;
}

void tt_set_is_harvestable(TTEntityId entity_id, bool harvestable) {
    assert(storage != NULL);
    return storage->set(entity_id, harvestable);
}

bool tt_is_harvestable(TTEntityId entity_id) {
    assert(storage != NULL);
    return storage->get(entity_id) != 0;
}
