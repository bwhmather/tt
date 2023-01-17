#include "tt-vector.h"

#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "tt-error.h"

void
tt_vector_init(TTVector *vector, size_t item_size) {
    tt_assert(vector != NULL);
    tt_assert(item_size > 0);

    vector->storage = NULL;
    vector->item_size = item_size;
    vector->item_count = 0;
    vector->item_capacity = 0;
}

void
tt_vector_destroy(TTVector *vector) {
    tt_assert(vector != NULL);

    free(vector->storage);
    vector->storage = NULL;

    vector->item_size = 0;
    vector->item_count = 0;
    vector->item_capacity = 0;
}

size_t
tt_vector_item_size(TTVector *vector) {
    tt_assert(vector != NULL);

    return vector->item_size;
}

size_t
tt_vector_item_count(TTVector *vector) {
    tt_assert(vector != NULL);

    return vector->item_count;
}

void
tt_vector_reserve(TTVector *vector, size_t item_count) {
    tt_assert(vector != NULL);

    if (item_count <= vector->item_count) {
        return;
    }

    if (vector->item_capacity == 0) {
        vector->item_capacity = 1024;
    }
    while (item_count > vector->item_capacity) {
        vector->item_capacity += vector->item_capacity / 2;
    }

    vector->storage =
        realloc(vector->storage, vector->item_size * vector->item_capacity);
    tt_assert(vector->storage != NULL);
}

void
tt_vector_shrink_to_fit(TTVector *vector) {
    // TODO
    (void)vector;
}

void
tt_vector_clear(TTVector *vector) {
    tt_assert(vector != NULL);
    vector->item_count = 0;
}

void
tt_vector_copy(TTVector *src, TTVector *tgt) {
    tt_assert(src != NULL);
    tt_assert(tgt != NULL);

    tt_assert(false); // Not implemented.
}

void *
tt_vector_push(TTVector *vector) {
    tt_assert(vector != NULL);

    tt_vector_reserve(vector, vector->item_count + 1);

    void *result = (void *)&((char *)vector->storage
    )[vector->item_count * vector->item_size];

    vector->item_count++;
    tt_assert(vector->item_count <= vector->item_capacity);

    return result;
}

void
tt_vector_pop(TTVector *vector) {
    tt_assert(vector != NULL);
    tt_assert(vector->item_count > 0);

    vector->item_count -= 1;
}

void
tt_vector_remove(TTVector *vector, ptrdiff_t index) {
    tt_assert(vector != NULL);

    size_t offset = index < 0 ? vector->item_count + index : (size_t)index;
    tt_assert(offset < vector->item_count);

    char *storage_bytes = (char *)vector->storage;

    char *item_start = &storage_bytes[vector->item_size * offset];
    char *tail_start =
        &storage_bytes[vector->item_size * offset + vector->item_size];
    char *tail_end = &storage_bytes[vector->item_size * vector->item_count];

    tt_assert(tail_end >= tail_start);
    tt_assert(tail_start > item_start);

    if (tail_end != tail_start) {
        memmove(item_start, tail_start, tail_end - tail_start);
    }

    vector->item_count -= 1;
}

void *
tt_vector_get(TTVector *vector, ptrdiff_t index) {
    size_t offset = index < 0 ? vector->item_count + index : (size_t)index;
    tt_assert(offset < vector->item_count);

    return (void *)&((char *)vector->storage)[vector->item_size * offset];
}
