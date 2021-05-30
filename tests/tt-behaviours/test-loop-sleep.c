/**
 * Checks that `tt_entities_release_id` will make an id available for reuse.
 */
#include "tt-behaviour-loop.h"
#include "tt-behaviour-sleep.h"

#include "tt-test.h"


int main(void) {
    TTBehaviour *behaviour = tt_behaviour_loop(
        tt_behaviour_sleep(2)
    );

    void *stack = malloc(tt_behaviour_max_stack_size(behaviour));
    void *fp = (void *) &((char *) stack)[tt_behaviour_max_stack_size(behaviour)];

    tt_assert(tt_behaviour_call(behaviour, 0, fp) == TT_BEHAVIOUR_RUNNING);
    tt_assert(tt_behaviour_call(behaviour, 0, fp) == TT_BEHAVIOUR_RUNNING);
    tt_assert(tt_behaviour_call(behaviour, 0, fp) == TT_BEHAVIOUR_RUNNING);

    tt_behaviour_free(behaviour);

    return 0;
}

