#pragma once

extern "C" {
#include "tt-entities.h"
}

typedef struct {
    int grid_x;
    int grid_y;
    int grid_width;
    int grid_height;
} TTSprite;

void tt_component_sprite_startup(void);
void tt_component_sprite_shutdown(void);

void tt_component_sprite_add(TTEntityId, TTSprite sprite);
TTSprite& tt_component_sprite_add(TTEntityId entity);

bool tt_component_sprite_has(TTEntityId entity);

TTSprite& tt_component_sprite_get(TTEntityId entity);

void tt_component_sprite_remove(TTEntityId entity);
