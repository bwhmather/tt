#include "tt-behaviour-sleep.h"

#include <malloc.h>
#include <stddef.h>
#include <string.h>

#include "tt-behaviour.h"
#include "tt-error.h"
#include "tt-entities.h"


typedef struct {
    TTBehaviour behaviour;
    unsigned int num_frames;
} TTBehaviourSleep;


typedef struct {
    unsigned int remaining_frames;
} TTBehaviourSleepState;


static TTBehaviourResult tt_behaviour_sleep_do_call(
    TTBehaviour *base, TTEntityId entity_id, void *fp
) {
    TTBehaviourSleep *behaviour = (TTBehaviourSleep *) base;
    TTBehaviourSleepState *state = (TTBehaviourSleepState *) fp;

    state->remaining_frames = behaviour->num_frames;

    if (state->remaining_frames == 0) {
        return TT_BEHAVIOUR_SUCCEEDED;
    }

    return TT_BEHAVIOUR_RUNNING;
}

static TTBehaviourResult tt_behaviour_sleep_do_resume(
    TTBehaviour *base, TTEntityId entity_id, void *fp
) {
    TTBehaviourSleep *behaviour = (TTBehaviourSleep *) base;
    TTBehaviourSleepState *state = (TTBehaviourSleepState *) fp;

    (void) behaviour;

    tt_assert(state->remaining_frames > 0);

    state->remaining_frames--;

    if (state->remaining_frames == 0) {
        return TT_BEHAVIOUR_SUCCEEDED;
    }

    return TT_BEHAVIOUR_RUNNING;
}

static void tt_behaviour_sleep_do_interrupt(
    TTBehaviour *base, TTEntityId entity_id, void *fp
) {
    (void) base;
    (void) entity_id;
    (void) fp;
}


static size_t tt_behaviour_sleep_max_stack_size(TTBehaviour *base) {
    return sizeof(TTBehaviourSleepState);
}


static void tt_behaviour_sleep_free(TTBehaviour *base) {
    TTBehaviourSleep *behaviour = (TTBehaviourSleep *) base;
    free(behaviour);
}


TTBehaviour *tt_behaviour_sleep(unsigned int num_frames) {
    TTBehaviourSleep *behaviour = (TTBehaviourSleep *) malloc(
        sizeof(TTBehaviourSleep)
    );
    tt_assert(behaviour != NULL);

    behaviour->behaviour = (TTBehaviour) {
        .do_call = tt_behaviour_sleep_do_call,
        .do_resume = tt_behaviour_sleep_do_resume,
        .do_interrupt = tt_behaviour_sleep_do_interrupt,

        .frame_size = sizeof(TTBehaviourSleepState),
        .compute_max_stack_size = tt_behaviour_sleep_max_stack_size,

        .free = tt_behaviour_sleep_free
    };

    behaviour->num_frames = num_frames;

    return (TTBehaviour *) behaviour;
}
