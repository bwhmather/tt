/**
 * Checks that adding and removing stuff from vector storage mostly works.
 */
#include "tt-entities.hpp"
#include "tt-storage-vector.hpp"

#include "tt-test.hpp"

#include <assert.h>
#include <stdint.h>


int main(void) {
    tt_entities_startup();

    TTStorageVector *storage = tt_storage_vector_new(sizeof(uint64_t));

    TTEntityId a_id = tt_entities_new_id();
    TTEntityId b_id = tt_entities_new_id();
    TTEntityId c_id = tt_entities_new_id();

    assert(tt_storage_vector_get(storage, a_id) == NULL);
    assert(tt_storage_vector_get(storage, b_id) == NULL);
    assert(tt_storage_vector_get(storage, c_id) == NULL);

    uint64_t *b_data = (uint64_t *) tt_storage_vector_add(storage, b_id);
    *b_data = 0xaaaaaaaaaaaaaaaa;

    assert(tt_storage_vector_get(storage, a_id) == NULL);
    assert((uint64_t *) tt_storage_vector_get(storage, b_id) == b_data);
    assert(tt_storage_vector_get(storage, c_id) == NULL);

    tt_storage_vector_free(storage);
    tt_entities_shutdown();

    return 0;
}

