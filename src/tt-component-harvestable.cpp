extern "C" {
#include "tt-component-harvestable.h"
}

#include "tt-storage-bitset.tpp"

extern "C" {
#include "tt-error.h"
}

namespace state {
    static bool initialised = false;
    static TTStorageBitset *storage = NULL;
}

extern "C" void tt_component_harvestable_startup(void) {
    tt_assert(state::initialised == false);

    state::storage = new TTStorageBitset();

    state::initialised = true;
}

extern "C" void tt_component_harvestable_shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;

    state::initialised = false;
}

extern "C" void tt_component_harvestable_set(
    TTEntityId entity_id, bool harvestable
) {
    tt_assert(state::initialised == true);

    return state::storage->set(entity_id, harvestable);
}

extern "C" bool tt_component_harvestable_get(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->get(entity_id);
}
