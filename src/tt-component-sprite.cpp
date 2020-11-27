#include "tt-component-sprite.hpp"

#include <assert.h>

#include "tt-storage-vector.tpp"


static TTStorageVector<TTSprite> *storage = NULL;

void tt_component_sprite_startup(void) {
    assert(storage == NULL);
    storage = new TTStorageVector<TTSprite>();
}

void tt_component_sprite_shutdown(void) {
    assert(storage != NULL);
    delete storage;
    storage = NULL;
}

void tt_add_sprite(TTEntityId entity_id, TTSprite sprite) {
    assert(storage != NULL);
    return storage->add(entity_id, sprite);
}

TTSprite &tt_add_sprite(TTEntityId entity_id) {
    assert(storage != NULL);
    TTSprite sprite = { 0, 0 };
    storage->add(entity_id, sprite);
    return storage->get(entity_id);
}

bool tt_has_sprite(TTEntityId entity_id) {
    assert(storage != NULL);
    return storage->has(entity_id);
}

TTSprite& tt_get_sprite(TTEntityId entity_id) {
    assert(storage != NULL);
    return storage->get(entity_id);
}

void tt_remove_sprite(TTEntityId entity_id) {
    assert(storage != NULL);
    return storage->remove(entity_id);
}

