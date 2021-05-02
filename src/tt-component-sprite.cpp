#include "tt-component-sprite.hpp"

#include "tt-error.hpp"
#include "tt-storage-sparse-vector.tpp"

namespace tt {
namespace component_sprite {

namespace state {
    static bool initialised = false;
    static TTStorageSparseVector<tt::Sprite> *storage = NULL;
}  /* namespace state */

void startup(void) {
    tt_assert(state::initialised == false);
    state::storage = new TTStorageSparseVector<tt::Sprite>();
    state::initialised = true;
}

void shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;
    state::initialised = false;
}

void add(TTEntityId entity_id, tt::Sprite sprite) {
    tt_assert(state::initialised == true);

    return state::storage->add(entity_id, sprite);
}

tt::Sprite &add(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    tt::Sprite sprite = { 0, 0 };
    state::storage->add(entity_id, sprite);
    return state::storage->get(entity_id);
}

bool has(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->has(entity_id);
}

tt::Sprite& get(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->get(entity_id);
}

void remove(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->remove(entity_id);
}

}  /* namespace component_sprite */
}  /* namespace tt */
