#include "tt-entity-iter.h"

#include "tt-entity-set.h"
#include "tt-entity-set-private.h"

#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>

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

