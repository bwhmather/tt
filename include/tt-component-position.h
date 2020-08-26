#pragma once

#include "tt-entities.h"

typedef struct {
    double x;
    double y;
} TTPosition;

void tt_component_position_startup(void);
void tt_component_position_shutdown(void);

TTPosition *tt_add_position(TTEntityId entity);
TTPosition *tt_get_position(TTEntityId entity);
void tt_remove_position(TTEntityId entity);

