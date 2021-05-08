#include "tt-renderer.hpp"

#include <cassert>
#include <cmath>
#include <cstdio>

#include "tt-component-move-to-target.hpp"
#include "tt-component-position.hpp"
#include "tt-component-target.hpp"
#include "tt-entities.hpp"
#include "tt-error.hpp"

namespace state {
    static bool initialised = false;
}

void tt_system_move_to_target_startup(void) {
    tt_assert(state::initialised == false);

    state::initialised = true;
}

void tt_system_move_to_target_shutdown(void) {
    tt_assert(state::initialised == true);

    state::initialised = false;
}

void tt_system_move_to_target_run(void) {
    TTEntityIter iter;

    tt_assert(state::initialised == true);

    tt_entities_iter_begin(&iter);

    while (tt_entities_iter_has_next(&iter)) {
        TTEntityId entity_id = tt_entities_iter_next(&iter);

        if (!tt_component_move_to_target_has(entity_id)) continue;
        if (!tt_component_target_has(entity_id)) {
            // warn();
            continue;
        }
        if (!tt_component_position_has(entity_id)) {
            // warn()
            continue;
        }
        //if (!tt_has_speed(entity_id)) {
            // warn()
          //  continue;
        //}
        double speed = 0.2;

        TTEntityId target_id = tt_component_target_get(entity_id);
        if (!tt_component_position_has(target_id)) {
            // warn()
            continue;
        }

        TTPosition &target_position = tt_component_position_get(target_id);
        TTPosition &position = tt_component_position_get(entity_id);

        double min_range = tt_component_move_to_target_get_target_range(
            entity_id
        );
        double current_range = std::sqrt(
            std::pow(position.x - target_position.x, 2) +
            std::pow(position.y - target_position.y, 2)
        );

        if (current_range < min_range) {
            tt_component_move_to_target_remove(entity_id);
        }

        double step = std::min(speed * 1 / 60, current_range);  // TODO

        position.x += (target_position.x - position.x) * step / current_range;
        position.y += (target_position.y - position.y) * step / current_range;
    }
}
