#pragma once

#include "bt.h"
#include "tt-entities.h"

void
tt_component_behaviour_startup(void);
void
tt_component_behaviour_shutdown(void);

void
tt_component_behaviour_set(TTEntityId entity_id, BTBehaviour *behaviour);
bool
tt_component_behaviour_has(TTEntityId entity_id);
BTBehaviour *
tt_component_behaviour_get(TTEntityId entity_id);
void
tt_component_behaviour_remove(TTEntityId entity_id);
