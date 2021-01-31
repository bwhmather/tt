#pragma once

#include "tt-entities.hpp"


void tt_component_target_startup(void);
void tt_component_target_shutdown(void);

void tt_set_target(TTEntityId, TTEntityId target);

bool tt_has_target(TTEntityId entity);

TTEntityId tt_get_target(TTEntityId entity);

void tt_remove_target(TTEntityId entity);
