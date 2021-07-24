#include "tt-behaviour-walk-to-target.h"

#include <malloc.h>
#include <math.h>
#include <stddef.h>

#include "tt-behaviour.h"
#include "tt-error.h"

#include "bt.h"
#include "tt-behaviour.h"
#include "tt-component-position.h"
#include "tt-component-target.h"
#include "tt-error.h"
#include "tt-entities.h"


static BTResult tt_behaviour_walk_to_target_tick(
    BTBehaviour *behaviour,
    void *state,
    TTBehaviourContext *context
) {
    (void) behaviour;
    (void) state;

    TTEntityId entity_id = context->entity_id;


    if (!tt_component_position_has(entity_id)) {
        return BT_FAILED;
    }
    if (!tt_component_target_has(entity_id)) {
        return BT_FAILED;
    }

    TTEntityId target_id = tt_component_target_get(entity_id);
    if (!tt_component_position_has(target_id)) {
        return BT_FAILED;
    }

    TTPosition *position = tt_component_position_get(entity_id);
    TTPosition *target = tt_component_position_get(target_id);

    double min_range = 0.04;
    double current_range = sqrt(
        pow(position->x - target->x, 2) +
        pow(position->y - target->y, 2)
    );

    if (current_range < min_range) {
        return BT_SUCCEEDED;
    }

    double speed = 0.2;
    double step = speed * 1.0f / 60.0f;
    if (step > current_range) {
        step = current_range;
    }

    position->x += (target->x - position->x) * step / current_range;
    position->y += (target->y - position->y) * step / current_range;

    return BT_RUNNING;
}


static void tt_behaviour_walk_to_target_free(BTBehaviour *behaviour) {
    free(behaviour);
}


BTBehaviour *tt_behaviour_walk_to_target(void) {
    BTBehaviour *behaviour = (BTBehaviour *) malloc(sizeof(BTBehaviour));
    tt_assert(behaviour != NULL);

    *behaviour = (BTBehaviour) {
        .init = NULL,
        .tick = (BTTickFn) tt_behaviour_walk_to_target_tick,
        .interrupt = NULL,

        .frame_size = 0,

        .free = tt_behaviour_walk_to_target_free
    };

    return behaviour;
}
