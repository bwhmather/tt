#pragma once

#include "tt-entities.hpp"

enum class TTGoal {
    NONE;
    COLLECT_WOOD;
    HARVEST_CROPS;
    CONSTRUCT_BUILDING;
    ATTACK;
    FLEE;
};

void tt_component_goal_startup(void);
void tt_component_goal_shutdown(void);

void tt_component_goal_set(TTEntityId, TTGoal goal);

bool tt_component_goal_has(TTEntityId entity);

TTGoal tt_component_goal_get(TTEntityId entity);

void tt_component_goal_remove(TTEntityId entity);
