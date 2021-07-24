#include "tt-vector.h"


typedef struct TTVector {
    void *storage;

    size_t item_size;
    size_t item_count;
    size_t item_capacity;
} TTVector;

void tt_vector_init(TTVector *vector, size_t item_size) {
    tt_assert(vector != NULL);

    vector->storage = NULL;
    vector->item_size = item_size;
    vector->item_count = 0;
    vector->item_capacity = 0;
}

void tt_vector_clear(TTVector *vector) {
    tt_assert(vector != NULL);

    free(vector->storage);
    vector->storage = NULL;

    vector->item_size = item_size;
    vector->item_count = 0;
    vector->item_capacity = 0;
}

void tt_vector_reserve(TTVector *vector, size_t item_count) {
    if (item_count >= vector->item_count) {
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
}

void tt_vector_shrink_to_fit(TTVector *vector) {
    // TODO
}

void tt_vector_copy(TTVector *src, TTVector *tgt) {
    tt_assert(src != NULL);
    tt_assert(tgt != NULL);

    tt_assert(false) // Not implemented.
}

void *tt_vector_push(TTVector *vector) {
    tt_assert(vector != NULL);


    bitset->storage[entity_id / 64] |= (((uint64_t) 1) << (entity_id % 64));

}

void tt_vector_pop(TTVector *vector) {

}

void tt_vector_remove(TTVector *vector, offset_t index);

void *tt_vector_get(TTVector *vector, offset_t index);
