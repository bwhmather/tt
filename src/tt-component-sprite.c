#include "tt-component-sprite.h"

#include "tt-entities.h"
#include "tt-error.h"
#include "tt-storage-vector.h"


static struct TTComponentSpriteState {
    bool initialised;
    TTStorageVector *storage;
} state = { .initialised = false };


void tt_component_sprite_startup(void) {
    tt_assert(state.initialised == false);

    state.storage = tt_storage_vector_new(sizeof(TTSprite));

    state.initialised = true;
}


void tt_component_sprite_shutdown(void) {
    tt_assert(state.initialised == true);

    tt_storage_vector_free(state.storage);
    state.storage = NULL;

    state.initialised = false;
}


TTSprite *tt_component_sprite_add(TTEntityId entity_id) {
    tt_assert(state.initialised == true);
    tt_assert(!tt_storage_vector_has(state.storage, entity_id));

    TTSprite *sprite = (TTSprite *) tt_storage_vector_add(
        state.storage, entity_id
    );

    *sprite = (TTSprite) {
        .grid_x = 0,
        .grid_y = 0,
        .grid_width = 0,
        .grid_height = 0,
    };

    return sprite;
}


bool tt_component_sprite_has(TTEntityId entity_id) {
    tt_assert(state.initialised == true);

    return tt_storage_vector_has(state.storage, entity_id);
}


TTSprite *tt_component_sprite_get(TTEntityId entity_id) {
    tt_assert(state.initialised == true);
    tt_assert(tt_storage_vector_has(state.storage, entity_id));

    return (TTSprite *) tt_storage_vector_get(state.storage, entity_id);
}


void tt_component_sprite_remove(TTEntityId entity_id) {
    tt_assert(state.initialised == true);
    tt_assert(tt_storage_vector_has(state.storage, entity_id));

    tt_storage_vector_remove(state.storage, entity_id);
}
