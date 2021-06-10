#include "tt-behaviour-fail.h"

#include <malloc.h>
#include <stddef.h>

#include "bt.h"
#include "tt-behaviour.h"
#include "tt-error.h"


static BTResult tt_behaviour_fail_init(
    BTBehaviour *behaviour,
    TTBehaviourContext *context,
    void *state
) {
    (void) behaviour;
    (void) context;

    return BT_FAILED;
}

static BTResult tt_behaviour_fail_tick(
    BTBehaviour *behaviour,
    TTBehaviourContext *context,
    void *state
) {
    (void) behaviour;
    (void) context;

    // This should never be called as call will never indicate that the
    // behaviour is still running.
    tt_assert(false);
}

static void tt_behaviour_fail_interrupt(
    BTBehaviour *behaviour,
    TTBehaviourContext *context,
    void *state
) {
    (void) behaviour;
    (void) context;

    // This should never be called as call will never indicate that the
    // behaviour is still running.
    tt_assert(false);
}


static void tt_behaviour_fail_free(BTBehaviour *behaviour) {
    free(behaviour);
}


BTBehaviour *tt_behaviour_fail(void) {
    BTBehaviour *behaviour = (BTBehaviour *) malloc(sizeof(BTBehaviour));
    tt_assert(behaviour != NULL);

    *behaviour = (BTBehaviour) {
        .init = (BTInitFn) tt_behaviour_fail_init,
        .tick = (BTTickFn) tt_behaviour_fail_tick,
        .interrupt = (BTInterruptFn) tt_behaviour_fail_interrupt,

        .frame_size = 0,

        .free = tt_behaviour_fail_free
    };

    return behaviour;
}
