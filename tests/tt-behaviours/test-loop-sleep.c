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

    BTContext *context = (BTContext *) malloc(5000);
    bt_context_init(context, 128);

    tt_assert(bt_run(behaviour, context, NULL) == BT_RUNNING);
    tt_assert(bt_run(behaviour, context, NULL) == BT_RUNNING);
    tt_assert(bt_run(behaviour, context, NULL) == BT_RUNNING);

    bt_run(NULL, context, NULL);
    bt_behaviour_free(behaviour);
    free(context);

    return 0;
}

