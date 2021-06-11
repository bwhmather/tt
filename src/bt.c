#include "bt.h"

#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>

#include "tt-error.h"


struct BTStack{
    size_t size;
    alignas(void *) char stack[];
};

typedef struct {
    BTBehaviour *behaviour;
    alignas(void *) char data[];
} BTFrame;


static struct BTState {
    BTStack *current_stack;
    BTFrame *next_frame;
    void *user_data;
} state = { .current_stack = NULL };


void bt_stack_init(BTStack *stack, size_t size) {
    // Must have at least enough space for a single stateless frame so that we
    // can use it as an immediate terminating null.
    tt_assert(size > sizeof(BTStack) + sizeof(BTFrame));

    stack->size = size;

    BTFrame *frame = (BTFrame *) stack->stack;
    frame->behaviour = NULL;
}

void bt_stack_shutdown(BTStack *stack, size_t size) {
    tt_assert(stack->size == size);

    BTFrame *frame = (BTFrame *) stack->stack;
    tt_assert(frame->behaviour == NULL);
}


BTResult bt_run(BTBehaviour *behaviour, BTStack *stack, void *user_data) {
    tt_assert(state.current_stack == NULL);

    state.current_stack = stack;
    state.next_frame = (BTFrame *) stack->stack;
    state.user_data = user_data;

    BTResult result = bt_delegate(behaviour);

    state.current_stack = NULL;
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
    tt_assert(state.current_stack != NULL);

    BTFrame *current_frame = state.next_frame;

    // If the stack doesn't already contain the state for the new behaviour,
    // clear whatever was there instead.  If the stack is empty then this is a
    // no-op.
    if (current_frame->behaviour != behaviour) {
        bt_interrupt(current_frame);
    }

    if (behaviour == NULL) {
        return BT_SUCCEEDED;
    }

    BTBehaviour *prev_behaviour = current_frame->behaviour;

    current_frame->behaviour = behaviour;
    state.next_frame = bt_next_frame(current_frame);

    tt_assert(
        state.current_stack->size >
        (uintptr_t) state.next_frame + sizeof(BTFrame) -
        (uintptr_t) state.current_stack
    );

    // Two possibilities:
    //   - Previous behaviour is NULL, either because it started as null or
    //     because it didn't match the new behaviour and was cleared.
    //   - Previous behaviour matches the new behaviour.
    if (prev_behaviour == NULL) {
        // Clear header of next frame so that we don't mistake it for active and
        // try to interrupt it when the current behaviour changes.
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
