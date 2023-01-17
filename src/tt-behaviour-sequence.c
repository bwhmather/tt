#include "tt-behaviour-sequence.h"

#include <malloc.h>
#include <stddef.h>
#include <string.h>

#include "bt.h"
#include "tt-behaviour.h"
#include "tt-entities.h"
#include "tt-error.h"

typedef struct {
    BTBehaviour behaviour;
    size_t num_children;
    BTBehaviour *children[];
} TTBehaviourSequence;

typedef struct {
    size_t active_child;
} TTBehaviourSequenceState;

static void
tt_behaviour_sequence_init(
    TTBehaviourSequence *behaviour, TTBehaviourSequenceState *state,
    TTBehaviourContext *context
) {
    (void)behaviour;
    (void)context;

    state->active_child = 0;
}

static BTResult
tt_behaviour_sequence_tick(
    TTBehaviourSequence *behaviour, TTBehaviourSequenceState *state,
    TTBehaviourContext *context
) {
    (void)context;

    while (state->active_child < behaviour->num_children) {
        BTResult result = bt_delegate(behaviour->children[state->active_child]);

        if (result == BT_RUNNING) {
            return result;
        }
        if (result == BT_FAILED) {
            return result;
        }

        state->active_child++;
    }

    return BT_SUCCEEDED;
}

static void
tt_behaviour_sequence_free(BTBehaviour *base) {
    TTBehaviourSequence *behaviour = (TTBehaviourSequence *)base;

    for (size_t child = 0; child < behaviour->num_children; child++) {
        bt_behaviour_free(behaviour->children[child]);
    }

    free(behaviour);
}

BTBehaviour *
tt_behaviour_sequence_from_array(BTBehaviour *children[]) {
    size_t num_children;
    for (num_children = 0; children[num_children] != NULL; num_children++) {
    }

    TTBehaviourSequence *behaviour = (TTBehaviourSequence *)malloc(
        sizeof(TTBehaviourSequence) + sizeof(BTBehaviour *) * (num_children + 1)
    );
    tt_assert(behaviour != NULL);

    behaviour->behaviour = (BTBehaviour
    ){.init = (BTInitFn)tt_behaviour_sequence_init,
      .tick = (BTTickFn)tt_behaviour_sequence_tick,
      .interrupt = NULL,

      .frame_size = sizeof(TTBehaviourSequenceState),

      .free = (BTFreeFn)tt_behaviour_sequence_free};

    behaviour->num_children = num_children;
    memcpy(behaviour->children, children, num_children * sizeof(BTBehaviour *));

    return (BTBehaviour *)behaviour;
}
