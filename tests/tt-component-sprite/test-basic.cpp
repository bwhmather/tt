#include "tt-component-sprite.hpp"
#include "tt-entities.hpp"

#include "tt-test.hpp"


int main(void) {
    tt::entities::startup();
    tt::component_sprite::startup();

    tt::EntityId entity_id = tt::entities::create();

    tt_assert(!tt::component_sprite::has(entity_id));

    tt::component_sprite::add(entity_id, {0, 5, 1, 2});

    tt_assert(tt::component_sprite::has(entity_id));

    tt::Sprite& sprite = tt::component_sprite::get(entity_id);
    tt_assert(sprite.grid_x == 0);
    tt_assert(sprite.grid_y == 5);
    tt_assert(sprite.grid_width == 1);
    tt_assert(sprite.grid_height == 2);

    tt::component_sprite::shutdown();
    tt::entities::shutdown();

    return 0;
}

