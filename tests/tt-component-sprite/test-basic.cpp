#include "tt-component-sprite.hpp"
#include "tt-entities.hpp"

#include "tt-test.hpp"


int main(void) {
    tt_entities_startup();
    tt_component_sprite_startup();

    TTEntityId entity_id = tt_entities_create();

    tt_assert(!tt_component_sprite_has(entity_id));

    tt_component_sprite_add(entity_id, {0, 5, 1, 2});

    tt_assert(tt_component_sprite_has(entity_id));

    TTSprite& sprite = tt_component_sprite_get(entity_id);
    tt_assert(sprite.grid_x == 0);
    tt_assert(sprite.grid_y == 5);
    tt_assert(sprite.grid_width == 1);
    tt_assert(sprite.grid_height == 2);

    tt_component_sprite_shutdown();
    tt_entities_shutdown();

    return 0;
}

