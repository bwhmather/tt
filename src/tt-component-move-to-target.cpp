#include "tt-component-move-to-target.hpp"

#include "tt-error.hpp"
#include "tt-storage-sparse-vector.tpp"

namespace tt {
namespace component_move_to_target {

namespace state {
    static bool initialised = false;
    static TTStorageSparseVector<double> *storage = NULL;
}  /* namespace state */

void startup(void) {
    tt_assert(state::initialised == false);

    state::storage = new TTStorageSparseVector<double>();
    tt_assert(state::storage != NULL);

    state::initialised = true;
}

void shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;

    state::initialised = false;
}

void set_target_range(TTEntityId entity_id, double range) {
    tt_assert(state::initialised == true);

    state::storage->add(entity_id, range);
}

bool has(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->has(entity_id);
}

double get_target_range(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->get(entity_id);
}

void remove(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    state::storage->remove(entity_id);
}

}  /* namespace component_move_to_storage */
}  /* namespace tt */

