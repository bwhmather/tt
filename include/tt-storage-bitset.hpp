#pragma once

#include "tt-entities.hpp"

#include <stdbool.h>

typedef struct TTStorageBitSet TTStorageBitSet;

TTStorageBitSet *tt_storage_bitset_new(void);
void tt_storage_bitset_free(TTStorageBitSet* entity_set);

void tt_storage_bitset_add(TTStorageBitSet *entity_set, TTEntityId entity);
void tt_storage_bitset_remove(TTStorageBitSet *entity_set, TTEntityId entity);
bool tt_storage_bitset_contains(TTStorageBitSet *entity_set, TTEntityId entity);

