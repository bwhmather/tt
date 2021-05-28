#pragma once

extern "C" {
#include "tt-behaviour.h"
#include "tt-entities.h"
}

void tt_component_behaviour_startup(void);
void tt_component_behaviour_shutdown(void);

void tt_component_behaviour_set_prev(TTEntityId entity_id, TTBehaviour *behaviour);
void tt_component_behaviour_set_next(TTEntityId entity_id, TTBehaviour *behaviour);

bool tt_component_behaviour_has(TTEntityId entity_id);

TTBehaviour *tt_component_behaviour_get_prev(TTEntityId entity_id);
TTBehaviour *tt_component_behaviour_get_next(TTEntityId entity_id);
void *tt_component_behaviour_get_stack(TTEntityId entity_id);

void tt_component_behaviour_remove(TTEntityId entity_id);
