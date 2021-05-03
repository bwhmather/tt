#pragma once

#include "tt-entities.hpp"

namespace tt {

enum class Job {
    ATTACK,
    FLEE
};

namespace component_job {

void startup(void);
void shutdown(void);

void set(tt::EntityId, tt::Job job);

bool has(tt::EntityId entity);

tt::Job get(tt::EntityId entity);

void remove(tt::EntityId entity);

}  /* namespace component_job */
}  /* namespace tt */
