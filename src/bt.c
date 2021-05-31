

typedef enum {
    BT_RUNNING,
    BT_SUCCEEDED,
    BT_FAILED
} BTResult;

typedef struct BTBehaviour BTBehaviour;
typedef struct BTContext BTContext;

typedef void (*BTInitFn)(BTBehaviour *, BTContext *, void *);
typedef BTResult (*BTRunFn)(BTBehaviour *, BTContext *, void *);
typedef void (*BTInterruptFn)(BTBehaviour *, BTContext *, void *);
typedef void (*BTFreeFn)(BTBehaviour *);

struct TTBehaviour {
    BTInitFun init;
    BTRunFn run;
    BTInterruptFn interrupt;

    size_t frame_size;

    BTFreeFn free;
};



#include "tt-behaviour.h"

#include <stddef.h>

#include "tt-entities.h"


typedef struct {
    size_t size;
    alignas(max_align_t) char stack[];
} BTContext;

typedef struct {
    BTBehaviour *behaviour;
    alignas(max_align_t) char state[];
} BTFrame;


static struct BTState {
    BTContext *current_context;
    BTFrame *current_frame;
    void *user_data;
} state = { .current_context = NULL };


void bt_init_context(
    BTContext *context, size_t size
) {

}

bt_run(BTBehaviour *behaviour, BTContext *context, void *user_data) {
    tt_assert(state.current_context == NULL);

    state.current_context = context;
    state.current_frame = context->state;
    state.user_data = user_data;

    bt_delegate(behaviour);
}


BTResult bt_delegate(BTBehaviour *behaviour) {
    tt_assert(state.current_context != NULL);

    current_frame = state.next_frame;
    state.next_frame = (
        current_frame + sizeof(BTFrame) + current_frame->behaviour->frame_size
    );
    tt_assert(frame_ptr < state.current_context.something);

    // If there is already a different behaviour running at this level in the
    // stack then interrupt and clear it and all of its children.
    if (frame->behaviour != behaviour && frame->behaviour != NULL) {
        // TODO interrupt
    }

    if (frame->behaviour == NULL); {
        // Attempt to allocate enough stack space for the new behaviour.

        // Call init function (if set).

    // Bump a pointer or counter or something so that the next call will work.

    // Call run function and return result.
    result = frame->behaviour->run(
        frame->behaviour, current_frame->data, state.user_data
    );

    if (result != BT_RUNNING) {
        current_frame->behaviour = NULL;
    }

    state.next_frame = current_frame;

    return result;
}





bt_sequence_init(behaviour, void *context, void *state) {
    state->active_child = 0;
}

bt_sequence_run(behaviour, void *context, void *state) {
    while (state->active_child < behaviour->num_children) {
        BTResult result = bt_call(
            behaviour->children[state->active_child]
        );

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






