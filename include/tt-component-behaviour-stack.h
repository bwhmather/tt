#pragma once

#include "bt.h"
#include "tt-entities.h"


void tt_component_behaviour_stack_startup(void);
void tt_component_behaviour_stack_shutdown(void);

void tt_component_behaviour_stack_init(TTEntityId entity_id);
bool tt_component_behaviour_stack_has(TTEntityId entity_id);
BTContext *tt_component_behaviour_stack_get(TTEntityId entity_id);
void tt_component_behaviour_stack_remove(TTEntityId entity_id);
