#pragma once

#include "tt-entities.hpp"


namespace tt {
namespace component_move_to_target {

void startup(void);
void shutdown(void);

void set_target_range(tt::EntityId, double range);

bool has(tt::EntityId entity);

double get_target_range(tt::EntityId entity);

void remove(tt::EntityId entity);

}  /* namespace tt */
}  /* namespace component_move_to_target */
