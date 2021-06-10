/**
 * Checks that loop will repeat after the wrapped behaviour finishes.
 */
#include "bt.h"
#include "tt-behaviour-loop.h"
#include "tt-behaviour-sleep.h"

#include "tt-test.h"


int main(void) {
    BTBehaviour *behaviour = tt_behaviour_loop(
        tt_behaviour_sleep(2)
    );

    BTStack *stack = (BTStack *) malloc(5000);
    bt_stack_init(stack, 128);

    tt_assert(bt_run(behaviour, stack, NULL) == BT_RUNNING);
    tt_assert(bt_run(behaviour, stack, NULL) == BT_RUNNING);
    tt_assert(bt_run(behaviour, stack, NULL) == BT_RUNNING);

    bt_run(NULL, stack, NULL);
    bt_behaviour_free(behaviour);
    free(stack);

    return 0;
}

