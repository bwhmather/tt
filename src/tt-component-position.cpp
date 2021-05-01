#include "tt-component-position.hpp"

#include <assert.h>

#include "tt-storage-sparse-vector.tpp"

namespace tt{
namespace component_position {

namespace detail {

static bool initialised = false;
static TTStorageSparseVector<Position> *storage = NULL;

}  /* namespace detail */

void startup(void) {
    assert(detail::initialised == false);

    detail::storage = new TTStorageSparseVector<Position>();

    detail::initialised = true;
}

void shutdown(void) {
    assert(detail::initialised == true);

    delete detail::storage;
    detail::storage = NULL;

    detail::initialised = false;
}

void add(TTEntityId entity_id, Position position) {
    assert(detail::initialised == true);

    return detail::storage->add(entity_id, position);
}

Position &add(TTEntityId entity_id) {
    assert(detail::initialised == true);

    Position position = { 0, 0 };
    detail::storage->add(entity_id, position);

    return detail::storage->get(entity_id);
}

bool has(TTEntityId entity_id) {
    assert(detail::initialised == true);

    return detail::storage->has(entity_id);
}

Position& get(TTEntityId entity_id) {
    assert(detail::initialised == true);

    return detail::storage->get(entity_id);
}

void remove(TTEntityId entity_id) {
    assert(detail::initialised == true);

    return detail::storage->remove(entity_id);
}

}  /* namespace component_position */
}  /* namespace tt */
