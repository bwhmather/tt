/**
 * Checks that adding and removing stuff from vector storage mostly works.
 */
#include "tt-storage-vector.h"

#include <stdint.h>

#include "tt-entities.h"

#include "tt-test.h"


int main(void) {
    tt_entities_startup();

    TTStorageVector *storage = tt_storage_vector_new(sizeof(uint64_t));

    TTEntityId a_id = tt_entities_create();
    TTEntityId b_id = tt_entities_create();
    TTEntityId c_id = tt_entities_create();

    tt_assert(tt_storage_vector_get(storage, a_id) == NULL);
    tt_assert(tt_storage_vector_get(storage, b_id) == NULL);
    tt_assert(tt_storage_vector_get(storage, c_id) == NULL);

    uint64_t *b_data = (uint64_t *) tt_storage_vector_add(storage, b_id);
    *b_data = 0xaaaaaaaaaaaaaaaa;

    tt_assert(tt_storage_vector_get(storage, a_id) == NULL);
    tt_assert((uint64_t *) tt_storage_vector_get(storage, b_id) == b_data);
    tt_assert(tt_storage_vector_get(storage, c_id) == NULL);

    tt_storage_vector_free(storage);
    tt_entities_shutdown();

    return 0;
}

