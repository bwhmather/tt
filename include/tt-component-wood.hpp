#pragma once

#include "tt-entities.hpp"


void tt_component_wood_startup(void);
void tt_component_wood_shutdown(void);

void tt_set_wood(TTEntityId, int wood);

bool tt_has_wood(TTEntityId entity);

int tt_get_wood(TTEntityId entity);
