#include "bt.h"

#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>

#include "tt-error.h"


struct BTContext{
    size_t size;
    alignas(void *) char stack[];
};

typedef struct {
    BTBehaviour *behaviour;
    alignas(void *) char data[];
} BTFrame;


static struct BTState {
    BTContext *current_context;
    BTFrame *next_frame;
    void *user_data;
} state = { .current_context = NULL };


void bt_context_init(BTContext *context, size_t size) {
    // Must have at least enough space for a single stateless frame so that we
    // can use it as an immediate terminating null.
    tt_assert(size > sizeof(BTContext) + sizeof(BTFrame));

    context->size = size;

    BTFrame *frame = (BTFrame *) context->stack;
    frame->behaviour = NULL;
}

void bt_context_shutdown(BTContext *context, size_t size) {
    tt_assert(context->size == size);

    BTFrame *frame = (BTFrame *) context->stack;
    tt_assert(frame->behaviour == NULL);
}


BTResult bt_run(BTBehaviour *behaviour, BTContext *context, void *user_data) {
    tt_assert(state.current_context == NULL);

    state.current_context = context;
    state.next_frame = (BTFrame *) context->stack;
    state.user_data = user_data;

    BTResult result = bt_delegate(behaviour);

    state.current_context = NULL;
    state.next_frame = NULL;
    state.user_data = NULL;

    return result;
}


static BTFrame *bt_next_frame(BTFrame *current_frame) {
    uintptr_t fp = (uintptr_t) current_frame;

    // Frame header of the current frame.
    fp += sizeof(BTFrame);

    // Frame data of the current frame.
    fp += current_frame->behaviour->frame_size;

    // Align to minimum alignment of a void pointer.
    fp += alignof(void *) - fp % alignof(void *);

    return (BTFrame *) fp;
}


static void bt_interrupt(BTFrame *frame) {
    if (frame->behaviour == NULL) {
        return;
    }

    bt_interrupt(bt_next_frame(frame));

    if (frame->behaviour->interrupt) {
        frame->behaviour->interrupt(
            frame->behaviour, (void *) frame->data, state.user_data
        );
    }

    frame->behaviour = NULL;
}


BTResult bt_delegate(BTBehaviour *behaviour) {
    tt_assert(state.current_context != NULL);

    BTFrame *current_frame = state.next_frame;

    // If there is already a different behaviour running at this level in the
    // stack then interrupt and clear it and all of its children.
    if (
        current_frame->behaviour != behaviour &&
        current_frame->behaviour != NULL
    ) {
        bt_interrupt(current_frame);
    }

    if (behaviour == NULL) {
        return BT_SUCCEEDED;
    }

    if (current_frame->behaviour == NULL) {
        // The target behaviour was not previously running.  Initialise it.
        current_frame->behaviour = behaviour;

        state.next_frame = bt_next_frame(current_frame);

        // Clear header of next frame so that we don't mistake it for active.
        state.next_frame->behaviour = NULL;

        // Call init function (if set).
        if (behaviour->init != NULL) {
            behaviour->init(
                behaviour, (void *) current_frame->data, state.user_data
            );
        }
    }

    // Call run function and return result.
    BTResult result = behaviour->tick(
        behaviour, (void *) current_frame->data, state.user_data
    );

    if (result != BT_RUNNING) {
        current_frame->behaviour = NULL;
    }

    state.next_frame = current_frame;

    return result;
}

void bt_behaviour_free(BTBehaviour *behaviour) {
    behaviour->free(behaviour);
}
