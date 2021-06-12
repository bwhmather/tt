#pragma once

#include "tt-entities.h"

typedef struct TTStorageBitset TTStorageBitset;

TTStorageBitset *tt_storage_bitset_new();
void tt_storage_bitset_free(TTStorageBitset *storage);

void tt_storage_bitset_add(TTStorageBitset *storage, TTEntityId entity_id);
void tt_storage_bitset_remove(TTStorageBitset *storage, TTEntityId entity_id);
bool tt_storage_bitset_contains(TTStorageBitset *storage, TTEntityId entity_id);
