#include "tt-component-position.hpp"

#include <assert.h>

#include "tt-storage-vector.tpp"


static TTStorageVector<TTPosition> *storage = NULL;

void tt_component_position_startup(void) {
    assert(storage == NULL);
    storage = new TTStorageVector<TTPosition>();
}

void tt_component_position_shutdown(void) {
    assert(storage != NULL);
    delete storage;
    storage = NULL;
}

void tt_add_position(TTEntityId entity_id, TTPosition position) {
    assert(storage != NULL);
    return storage->add(entity_id, position);
}

TTPosition &tt_add_position(TTEntityId entity_id) {
    assert(storage != NULL);
    TTPosition position = { 0, 0 };
    storage->add(entity_id, position);
    return storage->get(entity_id);
}

bool tt_has_position(TTEntityId entity_id) {
    assert(storage != NULL);
    return storage->has(entity_id);
}

TTPosition& tt_get_position(TTEntityId entity_id) {
    assert(storage != NULL);
    return storage->get(entity_id);
}

void tt_remove_position(TTEntityId entity_id) {
    assert(storage != NULL);
    return storage->remove(entity_id);
}

