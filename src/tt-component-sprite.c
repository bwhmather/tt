#include "tt-component-sprite.h"

#include "tt-storage-vector.h"

#include <assert.h>


static TTStorageVector *tt_component_sprite_storage = NULL;


void tt_component_sprite_startup(void) {
    assert(tt_component_sprite_storage == NULL);

    tt_component_sprite_storage = tt_storage_vector_new(sizeof(TTSprite));
}

void tt_component_sprite_shutdown(void) {
    assert(tt_component_sprite_storage != NULL);

    tt_storage_vector_free(tt_component_sprite_storage);
    tt_component_sprite_storage = NULL;
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



