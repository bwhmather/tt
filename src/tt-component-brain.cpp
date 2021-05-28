#include "tt-component-brain.hpp"
extern "C" {
#include "tt-error.h"
}
#include "tt-storage-bitset.tpp"

namespace state {
    static bool initialised = false;
    static TTStorageBitset *storage = NULL;
}

void tt_component_brain_startup(void) {
    tt_assert(state::initialised == false);

    state::storage = new TTStorageBitset();

    state::initialised = true;
}

void tt_component_brain_shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;

    state::initialised = false;
}

void tt_component_brain_set(TTEntityId entity_id, bool brain) {
    tt_assert(state::initialised == true);

    return state::storage->set(entity_id, brain);
}

bool tt_component_brain_get(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->get(entity_id);
}
