#include "tt-system-sprites.h"

#include <stdbool.h>

#include "tt-component-position.h"
#include "tt-component-sprite.h"
#include "tt-entities.h"
#include "tt-error.h"
#include "tt-renderer.h"

static struct TTSystemSpritesState {
    bool initialised;
} state = {.initialised = false};

void
tt_system_sprites_startup(void) {
    tt_assert(state.initialised == false);
    state.initialised = true;
}

void
tt_system_sprites_shutdown(void) {
    tt_assert(state.initialised == true);
    state.initialised = false;
}

void
tt_system_sprites_run(void) {
    TTEntityIter iter;

    tt_assert(state.initialised == true);

    tt_entities_iter_begin(&iter);

    while (tt_entities_iter_has_next(&iter)) {
        TTVertex vertex;
        TTEntityId entity_id = tt_entities_iter_next(&iter);

        if (!tt_component_sprite_has(entity_id))
            continue;
        if (!tt_component_position_has(entity_id))
            continue;

        TTSprite *sprite = tt_component_sprite_get(entity_id);
        TTPosition *position = tt_component_position_get(entity_id);

        float real_width = 0.1 * sprite->grid_width;
        float real_height = 0.1 * sprite->grid_height;

        float pixel = 1.0 / 128;

        vertex.x = position->x - 0.5 * real_width;
        vertex.y = position->y;
        vertex.z = 0.0;
        vertex.u = 8 * sprite->grid_x * pixel;
        vertex.v = 8 * (sprite->grid_y + sprite->grid_height) * pixel;
        tt_renderer_push_vertex(&vertex);

        vertex.x = position->x + 0.5 * real_width;
        vertex.y = position->y;
        vertex.z = 0.0;
        vertex.u = 8 * (sprite->grid_x + sprite->grid_width) * pixel;
        vertex.v = 8 * (sprite->grid_y + sprite->grid_height) * pixel;
        tt_renderer_push_vertex(&vertex);

        vertex.x = position->x + 0.5 * real_width;
        vertex.y = position->y;
        vertex.z = real_height;
        vertex.u = 8 * (sprite->grid_x + sprite->grid_width) * pixel;
        vertex.v = 8 * sprite->grid_y * pixel;
        tt_renderer_push_vertex(&vertex);

        vertex.x = position->x + 0.5 * real_width;
        vertex.y = position->y;
        vertex.z = real_height;
        vertex.u = 8 * (sprite->grid_x + sprite->grid_width) * pixel;
        vertex.v = 8 * sprite->grid_y * pixel;
        tt_renderer_push_vertex(&vertex);

        vertex.x = position->x - 0.5 * real_width;
        vertex.y = position->y;
        vertex.z = real_height;
        vertex.u = 8 * sprite->grid_x * pixel;
        vertex.v = 8 * sprite->grid_y * pixel;
        tt_renderer_push_vertex(&vertex);

        vertex.x = position->x - 0.5 * real_width;
        vertex.y = position->y;
        vertex.z = 0.0;
        vertex.u = 8 * sprite->grid_x * pixel;
        vertex.v = 8 * (sprite->grid_y + sprite->grid_height) * pixel;
        tt_renderer_push_vertex(&vertex);
    }
}
