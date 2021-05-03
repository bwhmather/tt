#pragma once

#include "tt-entities.hpp"

namespace tt {
enum Job {
    attack,
    flee
};
}

void tt_component_job_startup(void);
void tt_component_job_shutdown(void);

void tt_set_job(tt::EntityId, tt::Job job);

bool tt_has_job(tt::EntityId entity);

tt::Job tt_get_job(tt::EntityId entity);

void tt_remove_job(tt::EntityId entity);
