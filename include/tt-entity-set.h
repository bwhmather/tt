#pragma once

#include "tt-entities.h"

#include <stdbool.h>

typedef struct TTEntitySet TTEntitySet;

TTEntitySet *tt_entity_set_new(void);
void tt_entity_set_free(TTEntitySet* entity_set);

void tt_entity_set_add(TTEntitySet *entity_set, TTEntityId entity);
void tt_entity_set_remove(TTEntitySet *entity_set, TTEntityId entity);
bool tt_entity_set_contains(TTEntitySet *entity_set, TTEntityId entity);


typedef struct TTEntityIter TTEntityIter;

TTEntityIter *tt_entity_iter_new(int nsets, ...);
void tt_entity_iter_free(TTEntityIter *iter);

void tt_entity_iter_begin(TTEntityIter *iter);
bool tt_entity_iter_is_finished(TTEntityIter* iter);
TTEntityId tt_entity_iter_get(TTEntityIter *iter);
void tt_entity_iter_next(TTEntityIter *iter);

