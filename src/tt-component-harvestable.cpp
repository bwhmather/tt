#include "tt-component-harvestable.hpp"

#include "tt-error.hpp"
#include "tt-storage-bitset.tpp"

namespace state {
    static bool initialised = false;
    static TTStorageBitset *storage = NULL;
}

void tt_component_harvestable_startup(void) {
    tt_assert(state::initialised == false);

    state::storage = new TTStorageBitset();

    state::initialised = true;
}

void tt_component_harvestable_shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;

    state::initialised = false;
}

void tt_component_harvestable_set(TTEntityId entity_id, bool harvestable) {
    tt_assert(state::initialised == true);

    return state::storage->set(entity_id, harvestable);
}

bool tt_component_harvestable_get(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->get(entity_id);
}
