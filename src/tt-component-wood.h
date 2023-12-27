#pragma once

#include "tt-entities.h"

void
tt_component_wood_startup(void);
void
tt_component_wood_shutdown(void);

void
tt_component_wood_set(TTEntityId, int wood);

bool
tt_component_wood_has(TTEntityId entity);

int
tt_component_wood_get(TTEntityId entity);

void
tt_component_wood_remove(TTEntityId entity);
