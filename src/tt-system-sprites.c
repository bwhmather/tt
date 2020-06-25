#include "tt-renderer-sprites.h"

#include "tt-entities.h"
#include "tt-component-position->h"
#include "tt-component-sprite.h"

#include <assert.h>



void tt_system_sprites_run(void) {
    for (TTEntityId = 0; ) {
        TTVertex vertex;

        TTSprite *sprite = tt_component_sprite_get(entity);
        if (sprite == NULL) continue;

        TTPosition *position = tt_component_position_get(entity);
        if (position == NULL) continue;

        vertex.x = position->x - 0.5 * sprite.width;
        vertex.y = position->y;
        vertex.z = 0.0;
        vertex.u = 0.0;
        vertex.v = 0.0;
        tt_renderer_push_vertex(&vertex)

        vertex.x = position->x + 0.5 * sprite.width;
        vertex.y = position->y;
        vertex.z = 0.0;
        vertex.u = 1.0;
        vertex.v = 0.0;
        tt_renderer_push_vertex(&vertex)

        vertex.x = position->x + 0.5 * sprite.width;
        vertex.y = position->y;
        vertex.z = sprite.height;
        vertex.u = 1.0;
        vertex.v = 1.0;
        tt_renderer_push_vertex(&vertex)

        vertex.x = position->x + 0.5 * sprite.width;
        vertex.y = position->y;
        vertex.z = sprite.height;
        vertex.u = 1.0;
        vertex.v = 1.0;
        tt_renderer_push_vertex(&vertex)

        vertex.x = position->x - 0.5 * sprite.width;
        vertex.y = position->y;
        vertex.z = sprite.height;
        vertex.u = 0.0;
        vertex.v = 1.0;
        tt_renderer_push_vertex(&vertex)

        vertex.x = position->x - 0.5 * sprite.width;
        vertex.y = position->y;
        vertex.z = 0.0;
        vertex.u = 0.0;
        vertex.v = 0.0;
        tt_renderer_push_vertex(&vertex)
    }
}
