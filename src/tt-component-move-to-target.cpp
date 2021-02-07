#include "tt-component-move-to-target.hpp"

#include <assert.h>

#include "tt-storage-sparse-vector.tpp"


static TTStorageSparseVector<double> *storage = NULL;

void tt_component_move_to_target_startup(void) {
    assert(storage == NULL);
    storage = new TTStorageSparseVector<double>();
}

void tt_component_move_to_target_shutdown(void) {
    assert(storage != NULL);
    delete storage;
    storage = NULL;
}

void tt_set_move_to_target(TTEntityId entity_id, double range) {
    assert(storage != NULL);
    return storage->add(entity_id, range);
}

bool tt_has_move_to_target(TTEntityId entity_id) {
    assert(storage != NULL);
    return storage->has(entity_id);
}

double tt_get_move_to_target_range(TTEntityId entity_id) {
    assert(storage != NULL);
    return storage->get(entity_id);
}

void tt_remove_move_to_target(TTEntityId entity_id) {
    assert(storage != NULL);
    return storage->remove(entity_id);
}

