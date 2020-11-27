#pragma once

#include "tt-entities.hpp"

typedef struct {
    double width;
    double height;
} TTSprite;

void tt_component_sprite_startup(void);
void tt_component_sprite_shutdown(void);

void tt_add_sprite(TTEntityId, TTSprite sprite);
TTSprite& tt_add_sprite(TTEntityId entity);

bool tt_has_sprite(TTEntityId entity);

TTSprite& tt_get_sprite(TTEntityId entity);

void tt_remove_sprite(TTEntityId entity);
