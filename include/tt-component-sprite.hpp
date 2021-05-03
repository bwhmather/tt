#pragma once

#include "tt-entities.hpp"

namespace tt {

typedef struct {
    int grid_x;
    int grid_y;
    int grid_width;
    int grid_height;
} Sprite;

namespace component_sprite {

void startup(void);
void shutdown(void);

void add(tt::EntityId, tt::Sprite sprite);
tt::Sprite& add(tt::EntityId entity);

bool has(tt::EntityId entity);

tt::Sprite& get(tt::EntityId entity);

void remove(tt::EntityId entity);

}  /* namespace component_sprite */
}  /* namespace tt */
