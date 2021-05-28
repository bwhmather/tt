#pragma once

#include "tt-entities.h"


void tt_component_target_startup(void);
void tt_component_target_shutdown(void);

void tt_component_target_set(TTEntityId, TTEntityId target);

bool tt_component_target_has(TTEntityId entity);

TTEntityId tt_component_target_get(TTEntityId entity);

void tt_component_target_remove(TTEntityId entity);
