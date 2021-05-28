#include "tt-component-sprite.hpp"
extern "C" {
#include "tt-error.h"
}
#include "tt-storage-sparse-vector.tpp"

namespace state {
    static bool initialised = false;
    static TTStorageSparseVector<TTSprite> *storage = NULL;
}

void tt_component_sprite_startup(void) {
    tt_assert(state::initialised == false);
    state::storage = new TTStorageSparseVector<TTSprite>();
    state::initialised = true;
}

void tt_component_sprite_shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;
    state::initialised = false;
}

void tt_component_sprite_add(TTEntityId entity_id, TTSprite sprite) {
    tt_assert(state::initialised == true);

    return state::storage->add(entity_id, sprite);
}

TTSprite &tt_component_sprite_add(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    TTSprite sprite = { 0, 0 };
    state::storage->add(entity_id, sprite);
    return state::storage->get(entity_id);
}

bool tt_component_sprite_has(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->has(entity_id);
}

TTSprite& tt_component_sprite_get(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->get(entity_id);
}

void tt_component_sprite_remove(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->remove(entity_id);
}
