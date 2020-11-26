#include "tt-component-sprite.hpp"

#include "tt-storage-vector.hpp"

#include <assert.h>


static TTStorageVector *tt_component_sprite_storage = NULL;
static int tt_component_sprite_release_handle = 0;


void tt_component_sprite_startup(void) {
    assert(tt_component_sprite_storage == NULL);

    tt_component_sprite_storage = tt_storage_vector_new(sizeof(TTSprite));

    tt_component_sprite_release_handle = tt_entities_bind_release_callback(
        tt_component_sprite_remove
    );
}

void tt_component_sprite_shutdown(void) {
    assert(tt_component_sprite_storage != NULL);

    tt_storage_vector_free(tt_component_sprite_storage);
    tt_component_sprite_storage = NULL;

    tt_entities_unbind_release_callback(tt_component_sprite_release_handle);
    tt_component_sprite_release_handle = 0;
}

TTSprite *tt_component_sprite_add(TTEntityId entity) {
    assert(tt_component_sprite_storage != NULL);

    return (TTSprite *) tt_storage_vector_add(
        tt_component_sprite_storage, entity
    );
}

TTSprite *tt_component_sprite_get(TTEntityId entity) {
    assert(tt_component_sprite_storage != NULL);

    return (TTSprite *) tt_storage_vector_get(
        tt_component_sprite_storage, entity
    );
}

void tt_component_sprite_remove(TTEntityId entity) {
    assert(tt_component_sprite_storage != NULL);

    tt_storage_vector_remove(tt_component_sprite_storage, entity);
}

