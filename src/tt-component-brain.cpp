extern "C" {
#include "tt-component-brain.h"
}

#include "tt-storage-bitset.tpp"

extern "C" {
#include "tt-error.h"
}

namespace state {
    static bool initialised = false;
    static TTStorageBitset *storage = NULL;
}

extern "C" void tt_component_brain_startup(void) {
    tt_assert(state::initialised == false);

    state::storage = new TTStorageBitset();

    state::initialised = true;
}

extern "C" void tt_component_brain_shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;

    state::initialised = false;
}

extern "C" void tt_component_brain_set(TTEntityId entity_id, bool brain) {
    tt_assert(state::initialised == true);

    return state::storage->set(entity_id, brain);
}

extern "C" bool tt_component_brain_get(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->get(entity_id);
}
