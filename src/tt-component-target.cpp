#include "tt-component-target.hpp"

#include <assert.h>

#include "tt-storage-sparse-vector.tpp"


static TTStorageSparseVector<TTEntityId> *storage = NULL;

void tt_component_target_startup(void) {
    assert(storage == NULL);
    storage = new TTStorageSparseVector<TTEntityId>();
}

void tt_component_target_shutdown(void) {
    assert(storage != NULL);
    delete storage;
    storage = NULL;
}

void tt_set_target(TTEntityId entity_id, TTEntityId target) {
    assert(storage != NULL);
    return storage->add(entity_id, target);
}

bool tt_has_target(TTEntityId entity_id) {
    assert(storage != NULL);
    return storage->has(entity_id);
}

TTEntityId tt_get_target(TTEntityId entity_id) {
    assert(storage != NULL);
    return storage->get(entity_id);
}

void tt_remove_target(TTEntityId entity_id) {
    assert(storage != NULL);
    return storage->remove(entity_id);
}

