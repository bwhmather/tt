#pragma once

#include "tt-entities.hpp"

typedef struct {
    double width;
    double height;
} TTSprite;

void tt_component_sprite_startup(void);
void tt_component_sprite_shutdown(void);

TTSprite *tt_component_sprite_add(TTEntityId entity);
TTSprite *tt_component_sprite_get(TTEntityId entity);
void tt_component_sprite_remove(TTEntityId entity);

