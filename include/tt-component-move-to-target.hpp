#pragma once

#include "tt-entities.hpp"


void tt_component_move_to_target_startup(void);
void tt_component_move_to_target_shutdown(void);

void tt_set_move_to_target(TTEntityId, double range);

bool tt_has_move_to_target(TTEntityId entity);

double tt_get_move_to_target_range(TTEntityId entity);

void tt_remove_move_to_target(TTEntityId entity);
