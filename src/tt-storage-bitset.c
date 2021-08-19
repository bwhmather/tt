#include "tt-storage-bitset.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "tt-bitset.h"
#include "tt-entities.h"
#include "tt-error.h"


struct TTStorageBitset {
    TTBitset bitset;
    int delete_callback_handle;
};


static void on_delete_callback(TTEntityId entity_id, void *data) {
    TTStorageBitset *storage = (TTStorageBitset *) data;

    tt_bitset_unset(&storage->bitset, entity_id);
}


TTStorageBitset *tt_storage_bitset_new(void) {
    TTStorageBitset *storage = (TTStorageBitset *) malloc(
        sizeof(TTStorageBitset)
    );
    tt_assert(storage != NULL);

    tt_bitset_init(&storage->bitset);

    storage->delete_callback_handle = tt_entities_bind_on_delete_callback(
        &on_delete_callback, storage
    );

    return storage;
}


void tt_storage_bitset_free(TTStorageBitset *storage) {
    tt_assert(storage != NULL);

    tt_entities_unbind_on_delete_callback(storage->delete_callback_handle);

    tt_bitset_destroy(&storage->bitset);

    free(storage);
}


bool tt_storage_bitset_contains(
    TTStorageBitset *storage, TTEntityId entity_id
) {
    tt_assert(storage != NULL);

    return tt_bitset_get(&storage->bitset, entity_id);
}


void tt_storage_bitset_add(TTStorageBitset *storage, TTEntityId entity_id) {
    tt_assert(storage != NULL);

    tt_bitset_set(&storage->bitset, entity_id);
}


void tt_storage_bitset_remove(
    TTStorageBitset *storage, TTEntityId entity_id
) {
    tt_assert(storage != NULL);
    tt_assert(!tt_bitset_get(&storage->bitset, entity_id));

    tt_bitset_unset(&storage->bitset, entity_id);
}

