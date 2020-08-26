#pragma once

#include "tt-entities.h"

typedef struct {
    double width;
    double height;
} TTSprite;

void tt_component_sprite_startup(void);
void tt_component_sprite_shutdown(void);

TTSprite *tt_add_sprite(TTEntityId entity);
TTSprite *tt_get_sprite(TTEntityId entity);
void tt_remove_sprite(TTEntityId entity);

