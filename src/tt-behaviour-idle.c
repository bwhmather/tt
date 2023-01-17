
#include "tt-behaviour-idle.h"

#include <malloc.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "bt.h"
#include "tt-behaviour.h"
#include "tt-component-position.h"
#include "tt-entities.h"
#include "tt-error.h"

typedef struct {
    double target_x;
    double target_y;
} TTBehaviourIdleState;

static void
tt_behaviour_idle_init(
    BTBehaviour *behaviour, TTBehaviourIdleState *state,
    TTBehaviourContext *context
) {
    (void)behaviour;
    (void)context;

    // Choose target.
    state->target_x = 2 * ((double)rand()) / ((double)RAND_MAX) - 1;
    state->target_y = 2 * ((double)rand()) / ((double)RAND_MAX) - 1;
}

static BTResult
tt_behaviour_idle_tick(
    BTBehaviour *behaviour, TTBehaviourIdleState *state,
    TTBehaviourContext *context
) {
    (void)behaviour;

    if (!tt_component_position_has(context->entity_id)) {
        return BT_FAILED;
    }

    TTPosition *position = tt_component_position_get(context->entity_id);

    double min_range = 0.01;
    double current_range = sqrt(
        pow(position->x - state->target_x, 2) +
        pow(position->y - state->target_y, 2)
    );

    if (current_range < min_range) {
        return BT_SUCCEEDED;
    }

    double speed = 0.2;
    double step = speed * 1.0f / 60.0f;
    if (step > current_range) {
        step = current_range;
    }

    position->x += (state->target_x - position->x) * step / current_range;
    position->y += (state->target_y - position->y) * step / current_range;

    return BT_RUNNING;
}

static void
tt_behaviour_idle_free(BTBehaviour *behaviour) {
    free(behaviour);
}

BTBehaviour *
tt_behaviour_idle(void) {
    BTBehaviour *behaviour = (BTBehaviour *)malloc(sizeof(BTBehaviour));
    tt_assert(behaviour != NULL);

    *behaviour = (BTBehaviour
    ){.init = (BTInitFn)tt_behaviour_idle_init,
      .tick = (BTTickFn)tt_behaviour_idle_tick,
      .interrupt = NULL,

      .frame_size = sizeof(TTBehaviourIdleState),

      .free = tt_behaviour_idle_free};

    return behaviour;
}
