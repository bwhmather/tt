#pragma once

#include "tt-entities.h"


void tt_component_brain_startup(void);
void tt_component_brain_shutdown(void);

void tt_component_brain_set(TTEntityId, bool brain);
bool tt_component_brain_get(TTEntityId entity);
