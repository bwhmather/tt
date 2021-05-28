extern "C" {
#include "tt-component-position.h"
}

#include "tt-storage-sparse-vector.tpp"

extern "C" {
#include "tt-error.h"
}


namespace state {
    static bool initialised = false;
    static TTStorageSparseVector<TTPosition> *storage = NULL;
}

extern "C" void tt_component_position_startup(void) {
    tt_assert(state::initialised == false);

    state::storage = new TTStorageSparseVector<TTPosition>();

    state::initialised = true;
}

extern "C" void tt_component_position_shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;

    state::initialised = false;
}

extern "C" TTPosition *tt_component_position_add(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    TTPosition position = { 0, 0 };
    state::storage->add(entity_id, position);

    return &state::storage->get(entity_id);
}

extern "C" bool tt_component_position_has(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->has(entity_id);
}

extern "C" TTPosition *tt_component_position_get(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return &state::storage->get(entity_id);
}

extern "C" void tt_component_position_remove(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->remove(entity_id);
}
