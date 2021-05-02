#include "tt-component-target.hpp"

#include <assert.h>

#include "tt-error.hpp"
#include "tt-storage-sparse-vector.tpp"

namespace tt {
namespace component_target {

namespace detail {
static bool initialised = false;
static TTStorageSparseVector<TTEntityId> *storage = NULL;
}  /* namespace detail */

void startup(void) {
    tt_assert(detail::initialised == false);

    detail::storage = new TTStorageSparseVector<TTEntityId>();

    detail::initialised = true;
}

void shutdown(void) {
    tt_assert(detail::initialised == true);

    delete detail::storage;
    detail::storage = NULL;

    detail::initialised = false;
}

void set(TTEntityId entity_id, TTEntityId target) {
    tt_assert(detail::initialised == true);

    return detail::storage->add(entity_id, target);
}

bool has(TTEntityId entity_id) {
    tt_assert(detail::initialised == true);

    return detail::storage->has(entity_id);
}

TTEntityId get(TTEntityId entity_id) {
    tt_assert(detail::initialised == true);

    return detail::storage->get(entity_id);
}

void remove(TTEntityId entity_id) {
    tt_assert(detail::initialised == true);

    return detail::storage->remove(entity_id);
}

}  /* namespace component_target */
}  /* namespace tt */
