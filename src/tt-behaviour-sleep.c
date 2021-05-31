#include "tt-behaviour-sleep.h"

#include <malloc.h>
#include <stddef.h>
#include <string.h>

#include "bt.h"
#include "tt-behaviour.h"
#include "tt-error.h"
#include "tt-entities.h"


typedef struct {
    BTBehaviour behaviour;
    unsigned int num_frames;
} TTBehaviourSleep;


typedef struct {
    unsigned int remaining_frames;
} TTBehaviourSleepState;


static void tt_behaviour_sleep_init(
    TTBehaviourSleep *behaviour,
    TTBehaviourSleepState *state,
    TTBehaviourContext *context
) {
    (void) context;

    state->remaining_frames = behaviour->num_frames;
}

static BTResult tt_behaviour_sleep_tick(
    TTBehaviourSleep *behaviour,
    TTBehaviourSleepState *state,
    TTBehaviourContext *context
) {
    (void) behaviour;
    (void) context;

    tt_assert(state->remaining_frames > 0);

    state->remaining_frames--;

    if (state->remaining_frames == 0) {
        return BT_SUCCEEDED;
    }

    return BT_RUNNING;
}


static void tt_behaviour_sleep_free(TTBehaviourSleep *behaviour) {
    free(behaviour);
}


BTBehaviour *tt_behaviour_sleep(unsigned int num_frames) {
    TTBehaviourSleep *behaviour = (TTBehaviourSleep *) malloc(
        sizeof(TTBehaviourSleep)
    );
    tt_assert(behaviour != NULL);

    behaviour->behaviour = (BTBehaviour) {
        .init = (BTInitFn) tt_behaviour_sleep_init,
        .tick = (BTTickFn) tt_behaviour_sleep_tick,
        .interrupt = NULL,

        .frame_size = sizeof(TTBehaviourSleepState),

        .free = (BTFreeFn) tt_behaviour_sleep_free
    };

    behaviour->num_frames = num_frames;

    return (BTBehaviour *) behaviour;
}
