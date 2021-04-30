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

void add(TTEntityId, tt::Sprite sprite);
tt::Sprite& add(TTEntityId entity);

bool has(TTEntityId entity);

tt::Sprite& get(TTEntityId entity);

void remove(TTEntityId entity);

}  /* namespace component_sprite */
}  /* namespace tt */
