#pragma once

#include "tt-entities.h"


void tt_component_harvestable_startup(void);
void tt_component_harvestable_shutdown(void);

void tt_component_harvestable_set(TTEntityId, bool harvestable);
bool tt_component_harvestable_get(TTEntityId entity);
