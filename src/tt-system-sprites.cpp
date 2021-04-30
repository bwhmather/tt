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

namespace detail {
    static bool initialised = false;
}

void startup(void) {
    tt_assert(detail::initialised == false);
    detail::initialised = true;
}

void shutdown(void) {
    tt_assert(detail::initialised == true);
    detail::initialised = false;
}

void run(void) {
    TTEntityIter iter;

    tt_assert(detail::initialised == true);

    tt_entities_iter_begin(&iter);

    while (tt_entities_iter_has_next(&iter)) {
        tt::renderer::vertex vertex;
        TTEntityId entity_id = tt_entities_iter_next(&iter);

        if (!tt_has_sprite(entity_id)) continue;
        if (!tt_has_position(entity_id)) continue;

        TTSprite& sprite = tt_get_sprite(entity_id);
        TTPosition& position = tt_get_position(entity_id);

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
