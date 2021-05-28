extern "C" {
#include "tt-component-sprite.h"
}

#include "tt-storage-sparse-vector.tpp"

extern "C" {
#include "tt-error.h"
}


namespace state {
    static bool initialised = false;
    static TTStorageSparseVector<TTSprite> *storage = NULL;
}

extern "C" void tt_component_sprite_startup(void) {
    tt_assert(state::initialised == false);
    state::storage = new TTStorageSparseVector<TTSprite>();
    state::initialised = true;
}

extern "C" void tt_component_sprite_shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;
    state::initialised = false;
}

extern "C" TTSprite *tt_component_sprite_add(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    TTSprite sprite = { 0, 0 };
    state::storage->add(entity_id, sprite);
    return &state::storage->get(entity_id);
}

extern "C" bool tt_component_sprite_has(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->has(entity_id);
}

extern "C" TTSprite *tt_component_sprite_get(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return &state::storage->get(entity_id);
}

extern "C" void tt_component_sprite_remove(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->remove(entity_id);
}
