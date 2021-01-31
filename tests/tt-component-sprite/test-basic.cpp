#include "tt-component-sprite.hpp"
#include "tt-entities.hpp"

#include "tt-test.hpp"


int main(void) {
    tt_entities_startup();
    tt_component_sprite_startup();

    TTEntityId entity_id = tt_entities_create();

    tt_assert(!tt_has_sprite(entity_id));

    tt_add_sprite(entity_id, {1, 2});

    tt_assert(tt_has_sprite(entity_id));

    TTSprite& sprite = tt_get_sprite(entity_id);
    tt_assert(sprite.width == 1);
    tt_assert(sprite.height == 2);

    tt_component_sprite_shutdown();
    tt_entities_shutdown();

    return 0;
}

