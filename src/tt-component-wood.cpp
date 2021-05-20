#include "tt-component-wood.hpp"

#include <assert.h>

#include "tt-storage-vector.tpp"


static TTStorageVector<TTEntityId> *storage = NULL;

void tt_component_wood_startup(void) {
    assert(storage == NULL);
    storage = new TTStorageVector<TTEntityId>();
}

void tt_component_wood_shutdown(void) {
    assert(storage != NULL);
    delete storage;
    storage = NULL;
}

void tt_set_wood(TTEntityId entity_id, int wood) {
    assert(storage != NULL);
    return storage->set(entity_id, wood);
}

bool tt_has_wood(TTEntityId entity_id) {
    assert(storage != NULL);
    return storage->get(entity_id) != 0;
}

int tt_get_wood(TTEntityId entity_id) {
    assert(storage != NULL);
    return storage->get(entity_id);
}
