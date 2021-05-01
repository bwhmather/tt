#pragma once

#include "tt-entities.hpp"


namespace tt {
namespace component_move_to_target {

void startup(void);
void shutdown(void);

void set_target_range(TTEntityId, double range);

bool has(TTEntityId entity);

double get_target_range(TTEntityId entity);

void remove(TTEntityId entity);

}  /* namespace tt */
}  /* namespace component_move_to_target */
