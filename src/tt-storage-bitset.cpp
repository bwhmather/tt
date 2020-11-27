#include "tt-storage-bitset.hpp"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "tt-entities.hpp"


struct TTStorageBitSet {
    size_t nblocks;
    uint64_t *mask;
};

TTStorageBitSet *tt_storage_bitset_new(void) {
    size_t nblocks = 8;

    TTStorageBitSet *storage_bitset = (TTStorageBitSet *) malloc(
        sizeof(TTStorageBitSet)
    );
    assert(storage_bitset != NULL);

    storage_bitset->mask = (uint64_t *) malloc(sizeof(uint64_t) * nblocks);
    assert(storage_bitset->mask != NULL);
    memset(storage_bitset->mask, 0, sizeof(uint64_t) * nblocks);

    storage_bitset->nblocks = nblocks;

    return storage_bitset;
}

void tt_storage_bitset_free(TTStorageBitSet* storage_bitset) {
    assert(storage_bitset != NULL);
    free(storage_bitset);
}

void tt_storage_bitset_add(
    TTStorageBitSet *storage_bitset, TTEntityId entity_id
) {
    assert(storage_bitset != NULL);

    size_t block = entity_id / 64;
    int bit = entity_id % 64;

    if (block >= storage_bitset->nblocks) {
        size_t old_nblocks = storage_bitset->nblocks;
        size_t new_nblocks = storage_bitset->nblocks;

        while (block >= new_nblocks) {
            new_nblocks += new_nblocks / 2;
        }

        /* Reallocate mask. */
        storage_bitset->mask = (uint64_t *) realloc(
            storage_bitset->mask,
            sizeof(TTStorageBitSet) +
            sizeof(uint64_t) * new_nblocks
        );
        assert(storage_bitset->mask);
        memset(
            &storage_bitset->mask[old_nblocks],
            0, sizeof(uint64_t) * (new_nblocks - old_nblocks)
        );
    }

    storage_bitset->mask[block] |= 1 << bit;
}

void tt_storage_bitset_remove(
    TTStorageBitSet *storage_bitset, TTEntityId entity_id
) {
    assert(storage_bitset != NULL);

    if (!tt_storage_bitset_contains(storage_bitset, entity_id)) {
        return;
    }

    size_t block = entity_id / 64;
    int bit = entity_id % 64;

    storage_bitset->mask[block] &= ~(1 << bit);
}

bool tt_storage_bitset_contains(
    TTStorageBitSet *storage_bitset, TTEntityId entity_id
) {
    assert(storage_bitset != NULL);

    size_t block = entity_id / 64;
    int bit = entity_id % 64;

    if (block >= storage_bitset->nblocks) {
        return false;
    }

    if (!(storage_bitset->mask[block] & (1 << bit))) {
        return false;
    }

    return true;
}

