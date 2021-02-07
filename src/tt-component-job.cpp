#include "tt-component-job.hpp"

#include <assert.h>

#include "tt-storage-sparse-vector.tpp"


static TTStorageSparseVector<TTJob> *storage = NULL;

void tt_component_job_startup(void) {
    assert(storage == NULL);
    storage = new TTStorageSparseVector<TTJob>();
}

void tt_component_job_shutdown(void) {
    assert(storage != NULL);
    delete storage;
    storage = NULL;
}

void tt_set_job(TTEntityId entity_id, TTJob job) {
    assert(storage != NULL);
    return storage->add(entity_id, job);
}

bool tt_has_job(TTEntityId entity_id) {
    assert(storage != NULL);
    return storage->has(entity_id);
}

TTJob tt_get_job(TTEntityId entity_id) {
    assert(storage != NULL);
    return storage->get(entity_id);
}

void tt_remove_job(TTEntityId entity_id) {
    assert(storage != NULL);
    return storage->remove(entity_id);
}

