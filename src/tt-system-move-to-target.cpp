#include "tt-renderer.hpp"

#include <cassert>
#include <cmath>
#include <cstdio>

#include "tt-component-move-to-target.hpp"
#include "tt-component-position.hpp"
#include "tt-component-target.hpp"
#include "tt-entities.hpp"
#include "tt-error.hpp"

namespace tt {
namespace system_move_to_target {

namespace state {
    static bool initialised = false;
}  /* namespace state */

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
        tt::EntityId entity_id = tt::entities::iter_next(&iter);

        if (!tt::component_move_to_target::has(entity_id)) continue;
        if (!tt::component_target::has(entity_id)) {
            // warn();
            continue;
        }
        if (!tt::component_position::has(entity_id)) {
            // warn()
            continue;
        }
        //if (!tt_has_speed(entity_id)) {
            // warn()
          //  continue;
        //}
        double speed = 0.2;

        tt::EntityId target_id = tt::component_target::get(entity_id);
        if (!tt::component_position::has(target_id)) {
            // warn()
            continue;
        }

        tt::Position &target_position = tt::component_position::get(target_id);
        tt::Position &position = tt::component_position::get(entity_id);

        double min_range = tt::component_move_to_target::get_target_range(
            entity_id
        );
        double current_range = std::sqrt(
            std::pow(position.x - target_position.x, 2) +
            std::pow(position.y - target_position.y, 2)
        );

        if (current_range < min_range) {
            tt::component_move_to_target::remove(entity_id);
        }

        double step = std::min(speed * 1 / 60, current_range);  // TODO

        position.x += (target_position.x - position.x) * step / current_range;
        position.y += (target_position.y - position.y) * step / current_range;
    }
}

}  /* namespace system_move_to_target */
}  /* namespace tt */
