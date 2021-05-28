#include "tt-behaviour-walk-to-target.h"

#include <malloc.h>
#include <stddef.h>

#include "tt-behaviour.h"
#include "tt-error.h"

/*
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
*/

static TTBehaviourResult tt_behaviour_walk_to_target_do_call(
    TTBehaviour *behaviour, TTEntityId entity_id, void *fp
) {
    // TODO
    return TT_BEHAVIOUR_SUCCEEDED;
}

static TTBehaviourResult tt_behaviour_walk_to_target_do_resume(
    TTBehaviour *behaviour, TTEntityId entity_id, void *fp
) {
    // TODO
    tt_assert(false);
}

static void tt_behaviour_walk_to_target_do_interrupt(
    TTBehaviour *behaviour, TTEntityId entity_id, void *fp) {
    return;
}


static size_t tt_behaviour_walk_to_target_max_stack_size(TTBehaviour *behaviour) {
    (void *) behaviour;
    return 0;
}


static void tt_behaviour_walk_to_target_free(TTBehaviour *behaviour) {
    free(behaviour);
}


TTBehaviour *tt_behaviour_walk_to_target(void) {
    TTBehaviour *behaviour = (TTBehaviour *) malloc(sizeof(TTBehaviour));
    tt_assert(behaviour != NULL);
    *behaviour = (TTBehaviour) {
        .do_call = tt_behaviour_walk_to_target_do_call,
        .do_resume = tt_behaviour_walk_to_target_do_resume,
        .do_interrupt = tt_behaviour_walk_to_target_do_interrupt,

        .frame_size = 0,
        .compute_max_stack_size = tt_behaviour_walk_to_target_max_stack_size,

        .free = tt_behaviour_walk_to_target_free
    };

    return behaviour;
}

