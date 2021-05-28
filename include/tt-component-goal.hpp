#pragma once

extern "C" {
#include "tt-entities.h"
}

enum class TTGoal {
    NONE,
    COLLECT_WOOD,
    HARVEST_CROPS,
    CONSTRUCT_BUILDING,
    ATTACK,
    FLEE,
};

void tt_component_goal_startup(void);
void tt_component_goal_shutdown(void);

void tt_component_goal_set(TTEntityId entity_id, TTGoal goal);

bool tt_component_goal_has(TTEntityId entity_id);

TTGoal tt_component_goal_get(TTEntityId entity_id);

void tt_component_goal_remove(TTEntityId entity_id);
