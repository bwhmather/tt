/**
 * Checks that things can be removed from a storage vector.
 */
#include "tt-entities.hpp"
#include "tt-storage-vector.hpp"

#include "tt-test.hpp"

#include <assert.h>
#include <stdint.h>


int main(void) {
    tt_entities_startup();

    TTStorageVector *storage = tt_storage_vector_new(sizeof(uint64_t));

    TTEntityId id = tt_entities_new_id();

    assert(tt_storage_vector_get(storage, id) == NULL);
    tt_storage_vector_add(storage, id);
    assert(tt_storage_vector_get(storage, id) != NULL);
    tt_storage_vector_remove(storage, id);
    assert(tt_storage_vector_get(storage, id) == NULL);

    tt_storage_vector_free(storage);
    tt_entities_shutdown();

    return 0;
}

