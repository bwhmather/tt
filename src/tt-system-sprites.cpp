#include "tt-system-sprites.hpp"

#include <cassert>
#include <cstdio>

#include "tt-component-position.hpp"
#include "tt-component-sprite.hpp"
#include "tt-entities.hpp"
#include "tt-error.hpp"
#include "tt-renderer.hpp"


namespace tt {
namespace system_sprites {

namespace state {
    static bool initialised = false;
}

void startup(void) {
    tt_assert(state::initialised == false);
    state::initialised = true;
}

void shutdown(void) {
    tt_assert(state::initialised == true);
    state::initialised = false;
}

void run(void) {
    tt::EntityIter iter;

    tt_assert(state::initialised == true);

    tt::entities::iter_begin(&iter);

    while (tt::entities::iter_has_next(&iter)) {
        tt::renderer::vertex vertex;
        tt::EntityId entity_id = tt::entities::iter_next(&iter);

        if (!tt::component_sprite::has(entity_id)) continue;
        if (!tt::component_position::has(entity_id)) continue;

        tt::Sprite& sprite = tt::component_sprite::get(entity_id);
        tt::Position& position = tt::component_position::get(entity_id);

        float real_width = 0.1 * sprite.grid_width;
        float real_height = 0.1 * sprite.grid_height;

        float pixel = 1.0 / 128;

        vertex.x = position.x - 0.5 * real_width;
        vertex.y = position.y;
        vertex.z = 0.0;
        vertex.u = 8 * sprite.grid_x * pixel;
        vertex.v = 8 * (sprite.grid_y + sprite.grid_height) * pixel;
        tt::renderer::push_vertex(&vertex);

        vertex.x = position.x + 0.5 * real_width;
        vertex.y = position.y;
        vertex.z = 0.0;
        vertex.u = 8 * (sprite.grid_x + sprite.grid_width) * pixel;
        vertex.v = 8 * (sprite.grid_y + sprite.grid_height) * pixel;
        tt::renderer::push_vertex(&vertex);

        vertex.x = position.x + 0.5 * real_width;
        vertex.y = position.y;
        vertex.z = real_height;
        vertex.u = 8 * (sprite.grid_x + sprite.grid_width) * pixel;
        vertex.v = 8 * sprite.grid_y * pixel;
        tt::renderer::push_vertex(&vertex);

        vertex.x = position.x + 0.5 * real_width;
        vertex.y = position.y;
        vertex.z = real_height;
        vertex.u = 8 * (sprite.grid_x + sprite.grid_width) * pixel;
        vertex.v = 8 * sprite.grid_y * pixel;
        tt::renderer::push_vertex(&vertex);

        vertex.x = position.x - 0.5 * real_width;
        vertex.y = position.y;
        vertex.z = real_height;
        vertex.u = 8 * sprite.grid_x * pixel;
        vertex.v = 8 * sprite.grid_y * pixel;
        tt::renderer::push_vertex(&vertex);

        vertex.x = position.x - 0.5 * real_width;
        vertex.y = position.y;
        vertex.z = 0.0;
        vertex.u = 8 * sprite.grid_x * pixel;
        vertex.v = 8 * (sprite.grid_y + sprite.grid_height) * pixel;
        tt::renderer::push_vertex(&vertex);
    }
}

}  /* namespace system_sprites */
}  /* namespace tt */
