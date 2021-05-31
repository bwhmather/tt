/**
 * Checks that `tt_entities_release_id` will make an id available for reuse.
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
    bt_init_context(context, 128);

    tt_assert(bt_run(behaviour, context, NULL) == BT_RUNNING);
    tt_assert(bt_run(behaviour, context, NULL) == BT_RUNNING);
    tt_assert(bt_run(behaviour, context, NULL) == BT_RUNNING);

    bt_behaviour_free(behaviour);

    return 0;
}

