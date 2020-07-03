#include "tt-entity-set.h"
#include "tt-entity-set-private.h"

#include "tt-entities.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

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

