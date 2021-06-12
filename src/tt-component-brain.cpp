extern "C" {
#include "tt-component-brain.h"
}

extern "C" {
#include "tt-error.h"
#include "tt-storage-bitset.h"
}

namespace state {
    static bool initialised = false;
    static TTStorageBitset *storage = NULL;
}

extern "C" void tt_component_brain_startup(void) {
    tt_assert(state::initialised == false);

    state::storage = tt_storage_bitset_new();

    state::initialised = true;
}

extern "C" void tt_component_brain_shutdown(void) {
    tt_assert(state::initialised == true);

    tt_storage_bitset_free(state::storage);
    state::storage = NULL;

    state::initialised = false;
}

extern "C" void tt_component_brain_set(TTEntityId entity_id, bool brain) {
    tt_assert(state::initialised == true);

    if (brain && !tt_storage_bitset_contains(state::storage, entity_id)) {
        tt_storage_bitset_add(state::storage, entity_id);
    }

    if (!brain && tt_storage_bitset_contains(state::storage, entity_id)) {
        tt_storage_bitset_remove(state::storage, entity_id);
    }
}

extern "C" bool tt_component_brain_get(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return tt_storage_bitset_contains(state::storage, entity_id);
}
