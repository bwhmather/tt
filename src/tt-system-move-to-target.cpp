#include "tt-renderer.hpp"

#include <cassert>
#include <cmath>
#include <cstdio>

#include "tt-component-move-to-target.hpp"
#include "tt-component-position.hpp"
#include "tt-component-target.hpp"
#include "tt-entities.hpp"


void tt_system_move_to_target_run(void) {
    TTEntityIter iter;

    tt_entities_iter_begin(&iter);

    while (tt_entities_iter_has_next(&iter)) {
        TTEntityId entity_id = tt_entities_iter_next(&iter);

        if (!tt_has_move_to_target(entity_id)) continue;
        if (!tt_has_target(entity_id)) {
            // warn();
            continue;
        }
        if (!tt_has_position(entity_id)) {
            // warn()
            continue;
        }
        //if (!tt_has_speed(entity_id)) {
            // warn()
          //  continue;
        //}
        double speed = 0.2;

        TTEntityId target_id = tt_get_target(entity_id);
        if (!tt_has_position(target_id)) {
            // warn()
            continue;
        }

        TTPosition &target_position = tt_get_position(target_id);
        TTPosition &position = tt_get_position(entity_id);

        double min_range = tt_get_move_to_target_range(entity_id);
        double current_range = std::sqrt(
            std::pow(position.x - target_position.x, 2) +
            std::pow(position.y - target_position.y, 2)
        );

        if (current_range < min_range) {
            tt_remove_move_to_target(entity_id);
        }

        double step = std::min(speed * 1 / 60, current_range);  // TODO

        position.x += (target_position.x - position.x) * step / current_range;
        position.y += (target_position.y - position.y) * step / current_range;
    }
}

