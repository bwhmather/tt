#include "tt-behaviour-idle.h"

#include <malloc.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#include "tt-behaviour.h"
#include "tt-component-position.h"
#include "tt-error.h"


typedef struct {
    double target_x;
    double target_y;
} TTBehaviourIdleState;


static TTBehaviourResult tt_behaviour_idle_move_towards_target(
    TTEntityId entity_id, TTBehaviourIdleState *state
) {
    if (!tt_component_position_has(entity_id)) {
        return TT_BEHAVIOUR_FAILED;
    }

    TTPosition *position = tt_component_position_get(entity_id);

    double min_range = 0.01;
    double current_range = sqrt(
        pow(position->x - state->target_x, 2) +
        pow(position->y - state->target_y, 2)
    );

    if (current_range < min_range) {
        return TT_BEHAVIOUR_SUCCEEDED;
    }

    double speed = 0.2;
    double step = speed * 1.0f / 60.0f;
    if (step > current_range) {
        step = current_range;
    }

    position->x += (state->target_x - position->x) * step / current_range;
    position->y += (state->target_y - position->y) * step / current_range;

    return TT_BEHAVIOUR_RUNNING;
}

static TTBehaviourResult tt_behaviour_idle_do_call(
    TTBehaviour *behaviour, TTEntityId entity_id, void *fp
) {
    (void) behaviour;
    TTBehaviourIdleState *state = (TTBehaviourIdleState *) state;

    // Choose target.
    state->target_x = 2 * ((double) rand())/((double) RAND_MAX) - 1;
    state->target_y = 2 * ((double) rand())/((double) RAND_MAX) - 1;

    // Move towards it.
    return tt_behaviour_idle_move_towards_target(entity_id, fp);
}

static TTBehaviourResult tt_behaviour_idle_do_resume(
    TTBehaviour *behaviour, TTEntityId entity_id, void *fp
) {
    (void) behaviour;
    TTBehaviourIdleState *state = (TTBehaviourIdleState *) fp;

    return tt_behaviour_idle_move_towards_target(entity_id, state);
}

static void tt_behaviour_idle_do_interrupt(
    TTBehaviour *behaviour, TTEntityId entity_id, void *fp
) {
    (void) behaviour;
    (void) entity_id;
    (void) fp;

    return;
}


static size_t tt_behaviour_idle_max_stack_size(TTBehaviour *behaviour) {
    (void) behaviour;
    return 0;
}


static void tt_behaviour_idle_free(TTBehaviour *behaviour) {
    free(behaviour);
}


TTBehaviour *tt_behaviour_idle(void) {
    TTBehaviour *behaviour = (TTBehaviour *) malloc(sizeof(TTBehaviour));
    tt_assert(behaviour != NULL);
    *behaviour = (TTBehaviour) {
        .do_call = tt_behaviour_idle_do_call,
        .do_resume = tt_behaviour_idle_do_resume,
        .do_interrupt = tt_behaviour_idle_do_interrupt,

        .frame_size = 0,
        .compute_max_stack_size = tt_behaviour_idle_max_stack_size,

        .free = tt_behaviour_idle_free
    };

    return behaviour;
}

