#include "tt-entity-set.h"
#include "tt-entity-set-private.h"

#include "tt-entities.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

TTEntitySet *tt_entity_set_new(void) {
    size_t nblocks = 8;

    TTEntitySet *entity_set = malloc(sizeof(TTEntitySet));
    assert(entity_set != NULL);

    entity_set->mask = (uint64_t *) malloc(sizeof(uint64_t) * nblocks);
    assert(entity_set->mask != NULL);
    memset(entity_set->mask, 0, sizeof(uint64_t) * nblocks);

    entity_set->nblocks = nblocks;

    return entity_set;
}

void tt_entity_set_free(TTEntitySet* entity_set) {
    assert(entity_set != NULL);
    free(entity_set);
}

void tt_entity_set_add(TTEntitySet *entity_set, TTEntityId entity_id) {
    assert(entity_set != NULL);

    size_t block = entity_id / 64;
    int bit = entity_id % 64;

    if (block >= entity_set->nblocks) {
        size_t old_nblocks = entity_set->nblocks;
        size_t new_nblocks = entity_set->nblocks;

        while (block >= new_nblocks) {
            new_nblocks += new_nblocks / 2;
        }

        /* Reallocate mask. */
        entity_set->mask = (uint64_t *) realloc(
            entity_set->mask,
            sizeof(TTEntitySet) +
            sizeof(uint64_t) * new_nblocks
        );
        assert(entity_set->mask);
        memset(
            &entity_set->mask[old_nblocks],
            0, sizeof(uint64_t) * (new_nblocks - old_nblocks)
        );
    }

    entity_set->mask[block] |= 1 << bit;
}

void tt_entity_set_remove(TTEntitySet *entity_set, TTEntityId entity_id) {
    assert(entity_set != NULL);

    if (!tt_entity_set_contains(entity_set, entity_id)) {
        return;
    }

    size_t block = entity_id / 64;
    int bit = entity_id % 64;

    entity_set->mask[block] &= ~(1 << bit);
}

bool tt_entity_set_contains(TTEntitySet *entity_set, TTEntityId entity_id) {
    assert(entity_set != NULL);

    size_t block = entity_id / 64;
    int bit = entity_id % 64;

    if (block >= entity_set->nblocks) {
        return false;
    }

    if (!(entity_set->mask[block] & (1 << bit))) {
        return false;
    }

    return true;
}


struct TTEntityIter {
    TTEntityId cursor;
    uint64_t mask;

    size_t min_nblocks;
    int sets_size;
    TTEntitySet *sets[];
};

TTEntityIter *tt_entity_iter_new(int nsets, ...) {
    va_list sets;
    TTEntityIter *iter;

    iter = malloc(sizeof(TTEntityIter) + sizeof(TTEntitySet) * nsets);
    assert(iter != NULL);

    iter->cursor = 0;
    iter->mask = 0;
    iter->sets_size = nsets;

    va_start(sets, nsets);
    for (int i = 0; i < nsets; i++) {
        iter->sets[i] = va_arg(sets, TTEntitySet *);
    }
    va_end(sets);

    return iter;
}

static inline void tt_entity_iter_rebuild_mask(TTEntityIter *iter) {
    int block = iter->cursor / 64;

    iter->mask = 0xffffffffffffffff;
    for (int i = 0; i < iter->sets_size; i++) {
        TTEntitySet *entity_set = iter->sets[i];
        assert(entity_set->nblocks > block);
        iter->mask &= entity_set->mask[block];
    }
}

static inline void tt_entity_iter_seek(TTEntityIter *iter) {
    while (true) {
        iter->cursor += 1;
        iter->mask >>= 1;

        if (iter->cursor / 64 >= iter->min_nblocks) {
            iter->cursor = 0;
            return;
        }

        if (!(iter->cursor % 64)) {
            tt_entity_iter_rebuild_mask(iter);
        }

        if (iter->mask & 1) {
            return;
        }
    }
}


void tt_entity_iter_begin(TTEntityIter *iter) {
    iter->cursor = 0;

    iter->min_nblocks = 0;
    for (int i = 0; i < iter->sets_size; i++) {
        TTEntitySet *entity_set = iter->sets[i];
        iter->min_nblocks =
            entity_set->nblocks < iter->min_nblocks ?
            entity_set->nblocks : iter->min_nblocks;
    }

    if (iter->min_nblocks) {
        tt_entity_iter_rebuild_mask(iter);
        tt_entity_iter_seek(iter);
    }
}


bool tt_entity_iter_is_finished(TTEntityIter* iter) {
    return iter->cursor != 0;
}

void tt_entity_iter_next(TTEntityIter *iter) {
    assert(iter->cursor != 0);
    tt_entity_iter_seek(iter);
}

TTEntityId tt_entity_iter_get(TTEntityIter *iter) {
    return iter->cursor;
}

