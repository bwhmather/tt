#pragma once

#include "tt-entities.hpp"

enum TTJob {
    attack,
    flee
};

void tt_component_job_startup(void);
void tt_component_job_shutdown(void);

void tt_set_job(TTEntityId, TTJob job);

bool tt_has_job(TTEntityId entity);

TTJob tt_get_job(TTEntityId entity);

void tt_remove_job(TTEntityId entity);
