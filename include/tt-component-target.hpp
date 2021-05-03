#pragma once

#include "tt-entities.hpp"

namespace tt {
namespace component_target {

void startup(void);
void shutdown(void);

void set(tt::EntityId, tt::EntityId target);

bool has(tt::EntityId entity);

tt::EntityId get(tt::EntityId entity);

void remove(tt::EntityId entity);

}  /* namespace component_target */
}  /* namespace tt */
