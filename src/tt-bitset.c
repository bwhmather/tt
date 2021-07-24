#include "tt-bitset.h"

#include <malloc.h>
#include <string.h>

#include "tt-entities.h"
#include "tt-error.h"


void tt_bitset_init(TTBitset *bitset) {
    tt_assert(bitset != NULL);

    bitset->storage = NULL;
    bitset->capacity = 0;
}

void tt_bitset_destroy(TTBitset *bitset) {
    tt_assert(bitset != NULL);

    free(bitset->storage);

    bitset->storage = NULL;
    bitset->capacity = 0;
}


void tt_bitset_clear(TTBitset *bitset) {
    tt_assert(bitset != NULL);

    if (bitset->storage == NULL) return;
    if (bitset->capacity == 0) return;

    memset(bitset->storage, 0, ((bitset->capacity - 1) / 64) + 1);
}


void tt_bitset_copy(TTBitset *src, TTBitset *tgt) {
    tt_assert(src != NULL);
    tt_assert(tgt != NULL);

    if (src->capacity == 0) {
        tt_bitset_clear(tgt);
        return;
    }

    size_t block_count = ((src->capacity - 1) / 64) + 1;

    if (tgt->capacity != src->capacity) {
        tgt->storage = (uint64_t *) realloc(
            tgt->storage, sizeof(uint64_t) * block_count
        );
        tt_assert(tgt->storage != NULL || src->capacity == 0);

        tgt->capacity = src->capacity;
    }

    memcpy(tgt->storage, src->storage, sizeof(uint64_t) * block_count);
}


void tt_bitset_set(TTBitset *bitset, TTEntityId entity_id) {
    tt_assert(bitset != NULL);

    if (entity_id >= bitset->capacity) {
        size_t new_capacity = bitset->capacity ? bitset->capacity : 1024;

        while (entity_id >= new_capacity) {
            new_capacity += new_capacity / 2;
        }

        size_t old_block_count =
            bitset->capacity
            ? ((bitset->capacity - 1) / 64) + 1
            : 0;
        size_t new_block_count = ((new_capacity - 1) / 64) + 1;

        bitset->storage = (uint64_t *) realloc(
            bitset->storage, sizeof(uint64_t) * new_block_count
        );
        tt_assert(bitset->storage != NULL);

        memset(
            &bitset->storage[old_block_count],
            0, sizeof(uint64_t) * (new_block_count - old_block_count)
        );

        bitset->capacity = new_capacity;
    }

    bitset->storage[entity_id / 64] |= (((uint64_t) 1) << (entity_id % 64));
}

void tt_bitset_unset(TTBitset *bitset, TTEntityId entity_id) {
    tt_assert(bitset != NULL);

    if (entity_id >= bitset->capacity) {
        return;
    }

    bitset->storage[entity_id / 64] &= ~(((uint64_t) 1) << (entity_id % 64));
}


bool tt_bitset_get(TTBitset *bitset, TTEntityId entity_id) {
    tt_assert(bitset != NULL);

    if (entity_id >= bitset->capacity) {
        return false;
    }

    if (!(
        bitset->storage[entity_id / 64] &
        (((uint64_t) 1) << (entity_id % 64))
    )) {
        return false;
    }

    return true;
}

