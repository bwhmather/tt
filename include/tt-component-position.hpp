#pragma once

#include "tt-entities.hpp"

typedef struct {
    double x;
    double y;
} TTPosition;

void tt_component_position_startup(void);
void tt_component_position_shutdown(void);

void tt_component_position_add(TTEntityId entity, TTPosition position);
TTPosition& tt_component_position_add(TTEntityId entity);

bool tt_component_position_has(TTEntityId entity);

TTPosition& tt_component_position_get(TTEntityId entity);

void tt_component_position_remove(TTEntityId entity);
