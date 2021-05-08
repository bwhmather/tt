#include "tt-component-job.hpp"

#include "tt-error.hpp"
#include "tt-storage-sparse-vector.tpp"

namespace state {
    static bool initialised = false;
    static TTStorageSparseVector<TTJob> *storage = NULL;
}

void tt_component_job_startup(void) {
    tt_assert(state::initialised == false);

    state::storage = new TTStorageSparseVector<TTJob>();

    state::initialised = true;
}

void tt_component_job_shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;

    state::initialised = false;
}

void tt_component_job_set(TTEntityId entity_id, TTJob job) {
    tt_assert(state::initialised == true);

    return state::storage->add(entity_id, job);
}

bool tt_component_job_has(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->has(entity_id);
}

TTJob tt_component_job_get(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->get(entity_id);
}

void tt_component_job_remove(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->remove(entity_id);
}
