#include "tt-renderer.hpp"

#include <cassert>
#include <cstdio>

#include "tt-component-position.hpp"
#include "tt-component-sprite.hpp"
#include "tt-entities.hpp"


void tt_system_sprites_run(void) {
    TTEntityIter iter;

    tt_entities_iter_begin(&iter);

    while (tt_entities_iter_has_next(&iter)) {
        TTVertex vertex;
        TTEntityId entity_id = tt_entities_iter_next(&iter);

        if (!tt_has_sprite(entity_id)) continue;
        if (!tt_has_position(entity_id)) continue;

        TTSprite& sprite = tt_get_sprite(entity_id);
        TTPosition& position = tt_get_position(entity_id);

        vertex.x = position.x - 0.5 * sprite.width;
        vertex.y = position.y;
        vertex.z = 0.0;
        vertex.u = 0.0;
        vertex.v = 0.0;
        tt_renderer_push_vertex(&vertex);

        vertex.x = position.x + 0.5 * sprite.width;
        vertex.y = position.y;
        vertex.z = 0.0;
        vertex.u = 1.0;
        vertex.v = 0.0;
        tt_renderer_push_vertex(&vertex);

        vertex.x = position.x + 0.5 * sprite.width;
        vertex.y = position.y;
        vertex.z = sprite.height;
        vertex.u = 1.0;
        vertex.v = 1.0;
        tt_renderer_push_vertex(&vertex);

        vertex.x = position.x + 0.5 * sprite.width;
        vertex.y = position.y;
        vertex.z = sprite.height;
        vertex.u = 1.0;
        vertex.v = 1.0;
        tt_renderer_push_vertex(&vertex);

        vertex.x = position.x - 0.5 * sprite.width;
        vertex.y = position.y;
        vertex.z = sprite.height;
        vertex.u = 0.0;
        vertex.v = 1.0;
        tt_renderer_push_vertex(&vertex);

        vertex.x = position.x - 0.5 * sprite.width;
        vertex.y = position.y;
        vertex.z = 0.0;
        vertex.u = 0.0;
        vertex.v = 0.0;
        tt_renderer_push_vertex(&vertex);
    }
}

