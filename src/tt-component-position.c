#include "tt-component-position.h"

#include "tt-storage-vector.h"

#include <assert.h>


static TTStorageVector *tt_component_position_storage = NULL;
static int tt_component_position_release_handle = 0;


void tt_component_position_startup(void) {
    assert(tt_component_position_storage == NULL);

    tt_component_position_storage = tt_storage_vector_new(sizeof(TTPosition));

    tt_component_position_release_handle = tt_entities_bind_release_callback(
        tt_remove_position
    );
}

void tt_component_position_shutdown(void) {
    assert(tt_component_position_storage != NULL);

    tt_storage_vector_free(tt_component_position_storage);
    tt_component_position_storage = NULL;

    tt_entities_unbind_release_callback(tt_component_position_release_handle);
    tt_component_position_release_handle = 0;
}

TTPosition *tt_add_position(TTEntityId entity) {
    assert(tt_component_position_storage != NULL);

    return (TTPosition *) tt_storage_vector_add(
        tt_component_position_storage, entity
    );
}

TTPosition *tt_get_position(TTEntityId entity) {
    assert(tt_component_position_storage != NULL);

    return (TTPosition *) tt_storage_vector_get(
        tt_component_position_storage, entity
    );
}

void tt_remove_position(TTEntityId entity) {
    assert(tt_component_position_storage != NULL);

    tt_storage_vector_remove(tt_component_position_storage, entity);
}

