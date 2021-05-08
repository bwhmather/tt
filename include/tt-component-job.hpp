#pragma once

#include "tt-entities.hpp"

enum class TTJob {
    ATTACK,
    FLEE
};

void tt_component_job_startup(void);
void tt_component_job_shutdown(void);

void tt_component_job_set(TTEntityId, TTJob job);

bool tt_component_job_has(TTEntityId entity);

TTJob tt_component_job_get(TTEntityId entity);

void tt_component_job_remove(TTEntityId entity);
