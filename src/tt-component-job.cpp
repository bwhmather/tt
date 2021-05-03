#include "tt-component-job.hpp"

#include <assert.h>

#include "tt-storage-sparse-vector.tpp"


static tt::StorageSparseVector<tt::Job> *storage = NULL;

void tt_component_job_startup(void) {
    assert(storage == NULL);
    storage = new tt::StorageSparseVector<tt::Job>();
}

void tt_component_job_shutdown(void) {
    assert(storage != NULL);
    delete storage;
    storage = NULL;
}

void tt_set_job(tt::EntityId entity_id, tt::Job job) {
    assert(storage != NULL);
    return storage->add(entity_id, job);
}

bool tt_has_job(tt::EntityId entity_id) {
    assert(storage != NULL);
    return storage->has(entity_id);
}

tt::Job tt_get_job(tt::EntityId entity_id) {
    assert(storage != NULL);
    return storage->get(entity_id);
}

void tt_remove_job(tt::EntityId entity_id) {
    assert(storage != NULL);
    return storage->remove(entity_id);
}

