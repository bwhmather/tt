#pragma once

#include "tt-entities.h"

#include <stdbool.h>

typedef struct TTEntitySet TTEntitySet;

TTEntitySet *tt_entity_set_new(void);
void tt_entity_set_free(TTEntitySet* entity_set);

void tt_entity_set_add(TTEntitySet *entity_set, TTEntityId entity);
void tt_entity_set_remove(TTEntitySet *entity_set, TTEntityId entity);
bool tt_entity_set_contains(TTEntitySet *entity_set, TTEntityId entity);

