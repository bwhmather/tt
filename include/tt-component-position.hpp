#pragma once

#include "tt-entities.hpp"

namespace tt {

typedef struct {
    double x;
    double y;
} Position;

namespace component_position {

void startup(void);
void shutdown(void);

void add(TTEntityId entity, Position position);
Position& add(TTEntityId entity);

bool has(TTEntityId entity);

Position& get(TTEntityId entity);

void remove(TTEntityId entity);

}  /* namespace component_position */
}  /* namespace tt */
