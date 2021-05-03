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

void add(tt::EntityId entity, Position position);
Position& add(tt::EntityId entity);

bool has(tt::EntityId entity);

Position& get(tt::EntityId entity);

void remove(tt::EntityId entity);

}  /* namespace component_position */
}  /* namespace tt */
