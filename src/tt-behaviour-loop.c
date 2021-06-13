#include "tt-behaviour-loop.h"

#include <malloc.h>
#include <stddef.h>
#include <string.h>

#include "bt.h"
#include "tt-behaviour.h"
#include "tt-error.h"
#include "tt-entities.h"

typedef struct {
    BTBehaviour behaviour;
    size_t num_children;
    BTBehaviour *children[];
} TTBehaviourLoop;


typedef struct {
    size_t active_child;
} TTBehaviourLoopState;


static void tt_behaviour_loop_init(
    TTBehaviourLoop *behaviour,
    TTBehaviourLoopState *state,
    TTBehaviourContext *context
) {
    (void) behaviour;
    (void) context;

    state->active_child = 0;
}


static BTResult tt_behaviour_loop_tick(
    TTBehaviourLoop *behaviour,
    TTBehaviourLoopState *state,
    TTBehaviourContext *context
) {
    (void) context;

    while (true) {
        BTResult result = bt_delegate(
            behaviour->children[state->active_child]
        );

        if (result == BT_RUNNING) {
            return result;
        }
        if (result == BT_FAILED) {
            return result;
        }

        state->active_child++;
        if (state->active_child >= behaviour->num_children) {
            state->active_child = 0;
        }
    }
}

static void tt_behaviour_loop_free(BTBehaviour *base) {
    TTBehaviourLoop *behaviour = (TTBehaviourLoop *) base;

    for (size_t child = 0; child < behaviour->num_children; child++) {
        bt_behaviour_free(behaviour->children[child]);
    }

    free(behaviour);
}


BTBehaviour *tt_behaviour_loop_from_array(BTBehaviour *children[]) {
    size_t num_children;
    for (num_children = 0; children[num_children] != NULL; num_children++) {}

    TTBehaviourLoop *behaviour = (TTBehaviourLoop *) malloc(
        sizeof(TTBehaviourLoop) +
        sizeof(BTBehaviour*) * (num_children + 1)
    );
    tt_assert(behaviour != NULL);

    behaviour->behaviour = (BTBehaviour) {
        .init = (BTInitFn) tt_behaviour_loop_init,
        .tick = (BTTickFn) tt_behaviour_loop_tick,
        .interrupt = NULL,

        .frame_size = sizeof(TTBehaviourLoopState),

        .free = tt_behaviour_loop_free
    };

    behaviour->num_children = num_children;
    memcpy(
        behaviour->children, children,
        num_children * sizeof(BTBehaviour*)
    );

    return (BTBehaviour*) behaviour;
}






