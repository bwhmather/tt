#include "tt-component-job.hpp"

#include "tt-error.hpp"
#include "tt-storage-sparse-vector.tpp"


namespace tt {
namespace component_job {

namespace state {
    static bool initialised = false;
    static tt::StorageSparseVector<tt::Job> *storage = NULL;
}  /* namespace state */

void startup(void) {
    tt_assert(state::initialised == false);

    state::storage = new tt::StorageSparseVector<tt::Job>();

    state::initialised = true;
}

void shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;

    state::initialised = false;
}

void set(tt::EntityId entity_id, tt::Job job) {
    tt_assert(state::initialised == true);

    return state::storage->add(entity_id, job);
}

bool has(tt::EntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->has(entity_id);
}

tt::Job get(tt::EntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->get(entity_id);
}

void remove(tt::EntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->remove(entity_id);
}

}  /* namespace component_job */
}  /* namespace tt */

