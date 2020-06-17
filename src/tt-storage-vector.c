#include "tt-storage-vector.h"

#include "tt-entities.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


struct TTStorageVector {
    size_t size;
    size_t count;
    uint64_t *mask;
    unsigned char *buffer;
};

TTStorageVector *tt_storage_vector_new(size_t component_size) {
    TTStorageVector *storage = (TTStorageVector *) malloc(
        sizeof(TTStorageVector)
    );

    storage->size = component_size;
    storage->count = 256;
    
    storage->mask = (uint64_t *) calloc(
        sizeof(uint64_t), ((storage->count - 1) / 64) + 1
    );
    storage->buffer = calloc(component_size, storage->count);

    return storage;
}

void tt_storage_vector_free(TTStorageVector *storage) {
    assert(storage != NULL);

    free(storage->mask);
    free(storage->buffer);

    free(storage);
}

bool tt_storage_vector_has(TTStorageVector *storage, TTEntityId entity_id) {
    assert(storage != NULL);

    if (entity_id >= storage->count) {
        return false;
    }

    if (!(storage->mask[entity_id / 64] & (1 << (entity_id % 64)))) {
        return false;
    }
    
    return true;
}

void *tt_storage_vector_add(TTStorageVector *storage, TTEntityId entity_id) {
    assert(storage != NULL);
    
    if (entity_id >= storage->count) {
        size_t new_count = storage->count;

        while (entity_id >= new_count) {
            new_count += new_count / 2;
        }

        /* Reallocate mask. */
        size_t old_mask_count = ((storage->count - 1) / 64) + 1;
        size_t new_mask_count = ((new_count - 1) / 64) + 1;

        storage->mask = (uint64_t *) realloc(
            storage->mask, sizeof(uint64_t) * new_mask_count
        );
        assert(storage->mask);
        memset(
            &storage->mask[old_mask_count],
            0, sizeof(uint64_t) * (new_mask_count - old_mask_count)
        );

        /* Reallocate buffer */
        storage->buffer = realloc(storage->buffer, storage->size * new_count);
        assert(storage->buffer);
        memset(
            &storage->buffer[storage->size * storage->count],
            0, storage->size * (new_count - storage->count)
        );
        
        storage->count = new_count;
    }

    storage->mask[entity_id / 64] |= (1 << (entity_id % 64));
    
    return (void *) &storage->buffer[storage->size * entity_id];
}

void *tt_storage_vector_get(TTStorageVector *storage, TTEntityId entity_id) {
    assert(storage != NULL);
    
    if (!tt_storage_vector_has(storage, entity_id)) {
        return NULL;
    }
    
    assert(entity_id > 0);
    assert(entity_id < storage->count);
    
    return &storage->buffer[storage->size * entity_id];
}

void tt_storage_vector_remove(TTStorageVector *storage, TTEntityId entity_id) {
    assert(storage != NULL);
    
    if (!tt_storage_vector_has(storage, entity_id)) {
        return;
    }

    storage->mask[entity_id / 64] &= ~(1 << (entity_id % 64));
    memset(&storage->buffer[storage->size * entity_id], 0xaa, storage->count);
}

