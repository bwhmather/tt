#include "tt-component-sprite.hpp"
#include "tt-entities.hpp"

#include "tt-test.hpp"


int main(void) {
    tt_entities_startup();
    tt::component_sprite::startup();

    TTEntityId entity_id = tt_entities_create();

    tt_assert(!tt::component_sprite::has(entity_id));

    tt::component_sprite::add(entity_id, {0, 5, 1, 2});

    tt_assert(tt::component_sprite::has(entity_id));

    tt::Sprite& sprite = tt::component_sprite::get(entity_id);
    tt_assert(sprite.grid_x == 0);
    tt_assert(sprite.grid_y == 5);
    tt_assert(sprite.grid_width == 1);
    tt_assert(sprite.grid_height == 2);

    tt::component_sprite::shutdown();
    tt_entities_shutdown();

    return 0;
}

