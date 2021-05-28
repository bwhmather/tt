#include "tt-behaviour-loop.h"

#include <malloc.h>
#include <stddef.h>
#include <string.h>

#include "tt-behaviour.h"
#include "tt-error.h"
#include "tt-entities.h"


typedef struct {
    TTBehaviour behaviour;
    size_t num_children;
    TTBehaviour *children[];
} TTBehaviourLoop;


typedef struct {
    size_t active_child;
} TTBehaviourLoopState;


static TTBehaviourResult tt_behaviour_loop_do_call(
    TTBehaviour *base, TTEntityId entity_id, void *fp
) {
    TTBehaviourLoop *behaviour = (TTBehaviourLoop *) base;
    TTBehaviourLoopState *state = (TTBehaviourLoopState *) fp;

    state->active_child = 0;
    while (true) {
        TTBehaviourResult result = tt_behaviour_call(
            behaviour->children[state->active_child], entity_id, fp
        );

        if (result == TT_BEHAVIOUR_RUNNING) {
            return result;
        }
        if (result == TT_BEHAVIOUR_FAILED) {
            return result;
        }

        state->active_child++;
        if (state->active_child >= behaviour->num_children) {
            state->active_child = 0;
        }
    }
}

static TTBehaviourResult tt_behaviour_loop_do_resume(
    TTBehaviour *base, TTEntityId entity_id, void *fp
) {
    TTBehaviourLoop *behaviour = (TTBehaviourLoop *) base;
    TTBehaviourLoopState *state = (TTBehaviourLoopState *) fp;

    TTBehaviourResult result = tt_behaviour_resume(
        behaviour->children[state->active_child], entity_id, fp
    );

    while (true) {
        if (result == TT_BEHAVIOUR_RUNNING) {
            return result;
        }
        if (result == TT_BEHAVIOUR_FAILED) {
            return result;
        }

        state->active_child++;
        if (state->active_child >= behaviour->num_children) {
            state->active_child = 0;
        }

        result = tt_behaviour_call(
            behaviour->children[state->active_child], entity_id, fp
        );
    }
}

static void tt_behaviour_loop_do_interrupt(
    TTBehaviour *base, TTEntityId entity_id, void *fp
) {
    TTBehaviourLoop *behaviour = (TTBehaviourLoop *) base;
    TTBehaviourLoopState *state = (TTBehaviourLoopState *) fp;

    tt_behaviour_interrupt(
        behaviour->children[state->active_child], entity_id, fp
    );
}


static size_t tt_behaviour_loop_max_stack_size(TTBehaviour *base) {
    TTBehaviourLoop *behaviour = (TTBehaviourLoop *) base;

    size_t max_child_stack_size = 0;
    for (size_t child = 0; child < behaviour->num_children; child++) {
        size_t child_stack_size = tt_behaviour_max_stack_size(
            behaviour->children[child]
        );

        if (child_stack_size > max_child_stack_size) {
            max_child_stack_size = child_stack_size;
        }
    }

    return sizeof(TTBehaviourLoopState) + max_child_stack_size;

}


static void tt_behaviour_loop_free(TTBehaviour *base) {
    TTBehaviourLoop *behaviour = (TTBehaviourLoop *) base;

    for (size_t child = 0; child < behaviour->num_children; child++) {
        tt_behaviour_free(behaviour->children[child]);
    }

    free(behaviour);
}


TTBehaviour *tt_behaviour_loop_from_array(TTBehaviour *children[]) {
    size_t num_children;
    for (num_children = 0; children[num_children] != NULL; num_children++) {}

    TTBehaviourLoop *behaviour = (TTBehaviourLoop *) malloc(
        sizeof(TTBehaviourLoop) +
        sizeof(TTBehaviour *) * (num_children + 1)
    );
    tt_assert(behaviour != NULL);

    behaviour->behaviour = (TTBehaviour) {
        .do_call = tt_behaviour_loop_do_call,
        .do_resume = tt_behaviour_loop_do_resume,
        .do_interrupt = tt_behaviour_loop_do_interrupt,

        .frame_size = sizeof(TTBehaviourLoopState),
        .compute_max_stack_size = tt_behaviour_loop_max_stack_size,

        .free = tt_behaviour_loop_free
    };

    behaviour->num_children = num_children;
    memcpy(
        &behaviour->children, children,
        num_children * sizeof(TTBehaviour *)
    );

    return (TTBehaviour *) behaviour;
}
