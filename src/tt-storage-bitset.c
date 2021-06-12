#include "tt-storage-bitset.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "tt-entities.h"
#include "tt-error.h"

struct TTStorageBitset {
    size_t count;
    uint64_t *mask;
    int delete_callback_handle;
};


static void on_delete_callback(TTEntityId entity_id, void *data) {
    TTStorageBitset *storage = (TTStorageBitset *) data;

    if (tt_storage_bitset_contains(storage, entity_id)) {
        tt_storage_bitset_remove(storage, entity_id);
    }
}


TTStorageBitset *tt_storage_bitset_new() {
    TTStorageBitset *storage = (TTStorageBitset *) malloc(
        sizeof(TTStorageBitset)
    );
    tt_assert(storage != NULL);

    storage->count = 256;

    storage->mask = (uint64_t *) calloc(
        sizeof(uint64_t), ((storage->count - 1) / 64) + 1
    );
    tt_assert(storage->mask != NULL);

    storage->delete_callback_handle = tt_entities_bind_on_delete_callback(
        &on_delete_callback, storage
    );

    return storage;
}


void tt_storage_bitset_free(TTStorageBitset *storage) {
    tt_assert(storage != NULL);

    tt_entities_unbind_on_delete_callback(storage->delete_callback_handle);

    free(storage->mask);

    free(storage);
}


void tt_storage_bitset_add(TTStorageBitset *storage, TTEntityId entity_id);
void tt_storage_bitset_remove(TTStorageBitset *storage, TTEntityId entity_id);

bool tt_storage_bitset_contains(
    TTStorageBitset *storage, TTEntityId entity_id
) {
    tt_assert(storage != NULL);

    if (entity_id >= storage->count) {
        return false;
    }

    if (
        !(storage->mask[entity_id / 64] &
        (((uint64_t) 1) << (entity_id % 64)))
    ) {
        return false;
    }

    return true;
}


void tt_storage_bitset_add(TTStorageBitset *storage, TTEntityId entity_id) {
    tt_assert(storage != NULL);

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
        tt_assert(storage->mask);
        memset(
            &storage->mask[old_mask_count],
            0, sizeof(uint64_t) * (new_mask_count - old_mask_count)
        );

        storage->count = new_count;
    }

    storage->mask[entity_id / 64] |= (((uint64_t) 1) << (entity_id % 64));
}


void tt_storage_bitset_remove(
    TTStorageBitset *storage, TTEntityId entity_id
) {
    tt_assert(storage != NULL);
    tt_assert(!tt_storage_bitset_contains(storage, entity_id));

    storage->mask[entity_id / 64] &= ~(((uint64_t) 1) << (entity_id % 64));
}

