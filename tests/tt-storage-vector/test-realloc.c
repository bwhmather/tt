/**
 * Checks that adding an awful lot of stuff to a vector doesn't break it.
 */
#include "tt-storage-vector.h"

#include <stdint.h>

#include "tt-entities.h"

#include "tt-test.h"


#define COUNT 10000

int main(void) {
    tt_entities_startup();

    TTStorageVector *storage = tt_storage_vector_new(sizeof(uint64_t));

    TTEntityId *ids = (TTEntityId *) malloc(sizeof(TTEntityId) * COUNT);
    tt_assert(ids != NULL);

    for (size_t i = 0; i < COUNT; i++) {
        ids[i] = tt_entities_create();

        if (i % 3) {
            uint64_t *data = (uint64_t *) tt_storage_vector_add(
                storage, ids[i]
            );
            *data = ((uint64_t) i) + ((~((uint64_t) i)) << 32);
        }
    }

    for (size_t i = 0; i < COUNT; i++) {
        if (i % 3) {
            uint64_t *data = (uint64_t *) tt_storage_vector_get(
                storage, ids[i]
            );
            tt_assert(*data == ((uint64_t) i) + ((~((uint64_t) i)) << 32));
        }
    }

    free(ids);
    tt_storage_vector_free(storage);
    tt_entities_shutdown();

    return 0;
}

