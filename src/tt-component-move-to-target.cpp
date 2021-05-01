#include "tt-component-move-to-target.hpp"

#include "tt-error.hpp"
#include "tt-storage-sparse-vector.tpp"

namespace tt {
namespace component_move_to_target {

namespace detail {
static bool initialised = false;
static TTStorageSparseVector<double> *storage = NULL;
}  /* namespace detail */

void startup(void) {
    tt_assert(detail::initialised == false);

    detail::storage = new TTStorageSparseVector<double>();
    tt_assert(detail::storage != NULL);

    detail::initialised = true;
}

void shutdown(void) {
    tt_assert(detail::initialised == true);

    delete detail::storage;
    detail::storage = NULL;

    detail::initialised = false;
}

void set_target_range(TTEntityId entity_id, double range) {
    tt_assert(detail::initialised == true);

    detail::storage->add(entity_id, range);
}

bool has(TTEntityId entity_id) {
    tt_assert(detail::initialised == true);

    return detail::storage->has(entity_id);
}

double get_target_range(TTEntityId entity_id) {
    tt_assert(detail::initialised == true);

    return detail::storage->get(entity_id);
}

void remove(TTEntityId entity_id) {
    tt_assert(detail::initialised == true);

    detail::storage->remove(entity_id);
}

}  /* namespace component_move_to_storage */
}  /* namespace tt */

