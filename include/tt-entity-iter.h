#pragma once

#include "tt-entities.h"
#include "tt-entity-set.h"

#include <stdbool.h>

typedef struct TTEntityIter TTEntityIter;

TTEntityIter *tt_entity_iter_new(int nsets, ...);
void tt_entity_iter_free(TTEntityIter *iter);

void tt_entity_iter_begin(TTEntityIter *iter);
bool tt_entity_iter_is_finished(TTEntityIter* iter);
TTEntityId tt_entity_iter_get(TTEntityIter *iter);
void tt_entity_iter_next(TTEntityIter *iter);

