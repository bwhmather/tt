#include "tt-component-position.hpp"

#include <assert.h>

#include "tt-storage-sparse-vector.tpp"

namespace tt{
namespace component_position {

namespace state {
    static bool initialised = false;
    static tt::StorageSparseVector<Position> *storage = NULL;
}  /* namespace state */

void startup(void) {
    assert(state::initialised == false);

    state::storage = new tt::StorageSparseVector<Position>();

    state::initialised = true;
}

void shutdown(void) {
    assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;

    state::initialised = false;
}

void add(tt::EntityId entity_id, Position position) {
    assert(state::initialised == true);

    return state::storage->add(entity_id, position);
}

Position &add(tt::EntityId entity_id) {
    assert(state::initialised == true);

    Position position = { 0, 0 };
    state::storage->add(entity_id, position);

    return state::storage->get(entity_id);
}

bool has(tt::EntityId entity_id) {
    assert(state::initialised == true);

    return state::storage->has(entity_id);
}

Position& get(tt::EntityId entity_id) {
    assert(state::initialised == true);

    return state::storage->get(entity_id);
}

void remove(tt::EntityId entity_id) {
    assert(state::initialised == true);

    return state::storage->remove(entity_id);
}

}  /* namespace component_position */
}  /* namespace tt */
