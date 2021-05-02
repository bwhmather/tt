#pragma once

#include "tt-entities.hpp"

namespace tt {
namespace component_target {

void startup(void);
void shutdown(void);

void set(TTEntityId, TTEntityId target);

bool has(TTEntityId entity);

TTEntityId get(TTEntityId entity);

void remove(TTEntityId entity);

}  /* namespace component_target */
}  /* namespace tt */
