extern "C" {
#include "tt-component-wood.h"
}

#include "tt-storage-sparse-vector.tpp"

extern "C" {
#include "tt-error.h"
}

namespace state {
    static bool initialised = false;
    static TTStorageSparseVector<int> *storage = NULL;
}

extern "C" void tt_component_wood_startup(void) {
    tt_assert(state::initialised == false);

    state::storage = new TTStorageSparseVector<int>();

    state::initialised = true;
}

extern "C" void tt_component_wood_shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;

    state::initialised = false;
}

extern "C" void tt_component_wood_set(TTEntityId entity_id, int wood) {
    tt_assert(state::initialised == true);

    return state::storage->add(entity_id, wood);
}

extern "C" bool tt_component_wood_has(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->has(entity_id);
}

extern "C" int tt_component_wood_get(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->get(entity_id);
}

extern "C" void tt_component_wood_remove(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->remove(entity_id);
}
