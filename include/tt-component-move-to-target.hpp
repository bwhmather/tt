#pragma once
extern "C" {
#include "tt-entities.h"
}

void tt_component_move_to_target_startup(void);
void tt_component_move_to_target_shutdown(void);

void tt_component_move_to_target_set_target_range(TTEntityId, double range);

bool tt_component_move_to_target_has(TTEntityId entity);

double tt_component_move_to_target_get_target_range(TTEntityId entity);

void tt_component_move_to_target_remove(TTEntityId entity);
