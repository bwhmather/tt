#pragma once

#include "tt-entities.hpp"


void tt_component_harvestable_startup(void);
void tt_component_harvestable_shutdown(void);

void tt_set_is_harvestable(TTEntityId, int harvestable);
bool tt_is_harvestable(TTEntityId entity);
