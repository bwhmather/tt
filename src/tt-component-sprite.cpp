#include "tt-component-sprite.hpp"

#include "tt-error.hpp"
#include "tt-storage-sparse-vector.tpp"

namespace tt {
namespace component_sprite {

namespace detail {

static bool initialised = false;
static TTStorageSparseVector<tt::Sprite> *storage = NULL;

}  /* namespace detail */

void startup(void) {
    tt_assert(detail::initialised == false);
    detail::storage = new TTStorageSparseVector<tt::Sprite>();
    detail::initialised = true;
}

void shutdown(void) {
    tt_assert(detail::initialised == true);

    delete detail::storage;
    detail::storage = NULL;
    detail::initialised = false;
}

void add(TTEntityId entity_id, tt::Sprite sprite) {
    tt_assert(detail::initialised == true);

    return detail::storage->add(entity_id, sprite);
}

tt::Sprite &add(TTEntityId entity_id) {
    tt_assert(detail::initialised == true);

    tt::Sprite sprite = { 0, 0 };
    detail::storage->add(entity_id, sprite);
    return detail::storage->get(entity_id);
}

bool has(TTEntityId entity_id) {
    tt_assert(detail::initialised == true);

    return detail::storage->has(entity_id);
}

tt::Sprite& get(TTEntityId entity_id) {
    tt_assert(detail::initialised == true);

    return detail::storage->get(entity_id);
}

void remove(TTEntityId entity_id) {
    tt_assert(detail::initialised == true);

    return detail::storage->remove(entity_id);
}

}  /* namespace component_sprite */
}  /* namespace tt */
