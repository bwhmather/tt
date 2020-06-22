#include "tt-component-position.h"

#include "tt-storage-vector.h"

#include <assert.h>


static TTStorageVector *tt_component_position_storage = NULL;


void tt_component_position_startup(void) {
    assert(tt_component_position_storage == NULL);

    tt_component_position_storage = tt_storage_vector_new(sizeof(TTPosition));
}

void tt_component_position_shutdown(void) {
    assert(tt_component_position_storage != NULL);

    tt_storage_vector_free(tt_component_position_storage);
    tt_component_position_storage = NULL;
}

TTPosition *tt_component_position_add(TTEntityId entity) {
    assert(tt_component_position_storage != NULL);

    return (TTPosition *) tt_storage_vector_add(
        tt_component_position_storage, entity
    );
}

TTPosition *tt_component_position_get(TTEntityId entity) {
    assert(tt_component_position_storage != NULL);

    return (TTPosition *) tt_storage_vector_get(
        tt_component_position_storage, entity
    );
}

void tt_component_position_remove(TTEntityId entity) {
    assert(tt_component_position_storage != NULL);

    tt_storage_vector_remove(tt_component_position_storage, entity);
}



