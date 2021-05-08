#include "tt-component-position.hpp"

#include <assert.h>

#include "tt-storage-sparse-vector.tpp"

namespace state {
    static bool initialised = false;
    static TTStorageSparseVector<TTPosition> *storage = NULL;
}

void tt_component_position_startup(void) {
    assert(state::initialised == false);

    state::storage = new TTStorageSparseVector<TTPosition>();

    state::initialised = true;
}

void tt_component_position_shutdown(void) {
    assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;

    state::initialised = false;
}

void tt_component_position_add(TTEntityId entity_id, TTPosition position) {
    assert(state::initialised == true);

    return state::storage->add(entity_id, position);
}

TTPosition &tt_component_position_add(TTEntityId entity_id) {
    assert(state::initialised == true);

    TTPosition position = { 0, 0 };
    state::storage->add(entity_id, position);

    return state::storage->get(entity_id);
}

bool tt_component_position_has(TTEntityId entity_id) {
    assert(state::initialised == true);

    return state::storage->has(entity_id);
}

TTPosition& tt_component_position_get(TTEntityId entity_id) {
    assert(state::initialised == true);

    return state::storage->get(entity_id);
}

void tt_component_position_remove(TTEntityId entity_id) {
    assert(state::initialised == true);

    return state::storage->remove(entity_id);
}
