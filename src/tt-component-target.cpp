#include "tt-component-target.hpp"

#include <assert.h>

#include "tt-error.hpp"
#include "tt-storage-sparse-vector.tpp"

namespace tt {
namespace component_target {

namespace state {
    static bool initialised = false;
    static tt::StorageSparseVector<tt::EntityId> *storage = NULL;
}  /* namespace state */

void startup(void) {
    tt_assert(state::initialised == false);

    state::storage = new tt::StorageSparseVector<tt::EntityId>();

    state::initialised = true;
}

void shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;

    state::initialised = false;
}

void set(tt::EntityId entity_id, tt::EntityId target) {
    tt_assert(state::initialised == true);

    return state::storage->add(entity_id, target);
}

bool has(tt::EntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->has(entity_id);
}

tt::EntityId get(tt::EntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->get(entity_id);
}

void remove(tt::EntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->remove(entity_id);
}

}  /* namespace component_target */
}  /* namespace tt */
