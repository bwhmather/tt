#pragma once

#include "tt-entities.h"

typedef struct TTBitset {
    uint64_t *storage;
    size_t capacity;
} TTBitset;

void tt_bitset_init(TTBitset *bitset);
void tt_bitset_clear(TTBitset *bitset);

void tt_bitset_copy(TTBitset *src, TTBitset *tgt);

void tt_bitset_set(TTBitset *bitset, TTEntityId entity_id);
void tt_bitset_unset(TTBitset *bitset, TTEntityId entity_id);

bool tt_bitset_get(TTBitset *bitset, TTEntityId entity_id);

