extern "C" {
#include "tt-component-target.h"
}

#include "tt-storage-sparse-vector.tpp"

extern "C" {
#include "tt-error.h"
}


namespace state {
    static bool initialised = false;
    static TTStorageSparseVector<TTEntityId> *storage = NULL;
}

extern "C" void tt_component_target_startup(void) {
    tt_assert(state::initialised == false);

    state::storage = new TTStorageSparseVector<TTEntityId>();

    state::initialised = true;
}

extern "C" void tt_component_target_shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;

    state::initialised = false;
}

extern "C" void tt_component_target_set(
    TTEntityId entity_id, TTEntityId target
) {
    tt_assert(state::initialised == true);

    return state::storage->add(entity_id, target);
}

extern "C" bool tt_component_target_has(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->has(entity_id);
}

extern "C" TTEntityId tt_component_target_get(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->get(entity_id);
}

extern "C" void tt_component_target_remove(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->remove(entity_id);
}
