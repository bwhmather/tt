#pragma once


typedef struct TTVector {
    void *storage;

    size_t item_size;
    size_t item_count;
    size_t item_capacity;
} TTVector;

void tt_vector_init(TTVector *vector, size_t item_size);
void tt_vector_clear(TTVector *vector);

void tt_vector_reserve(TTVector *vector, size_t item_count);
void tt_vector_shrink_to_fit(TTVector *vector);

void tt_vector_copy(TTVector *src, TTVector *tgt);

void *tt_vector_push(TTVector *vector);

void tt_vector_pop(TTVector *vector);

void tt_vector_remove(TTVector *vector, offset_t index);

void *tt_vector_get(TTVector *vector, offset_t index);
