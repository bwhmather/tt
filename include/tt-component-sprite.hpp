#pragma once

#include "tt-entities.hpp"

typedef struct {
    int grid_x;
    int grid_y;
    int grid_width;
    int grid_height;
} TTSprite;

void tt_component_sprite_startup(void);
void tt_component_sprite_shutdown(void);

void tt_add_sprite(TTEntityId, TTSprite sprite);
TTSprite& tt_add_sprite(TTEntityId entity);

bool tt_has_sprite(TTEntityId entity);

TTSprite& tt_get_sprite(TTEntityId entity);

void tt_remove_sprite(TTEntityId entity);
