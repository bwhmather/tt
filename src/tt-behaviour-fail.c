#include "tt-behaviour-fail.h"

#include <malloc.h>
#include <stddef.h>

#include "tt-behaviour.h"
#include "tt-error.h"


static TTBehaviourResult tt_behaviour_fail_do_call(
    TTBehaviour *behaviour, TTEntityId entity_id, void *fp
) {
    (void) behaviour;
    (void) entity_id;
    (void) fp;

    return TT_BEHAVIOUR_FAILED;
}

static TTBehaviourResult tt_behaviour_fail_do_resume(
    TTBehaviour *behaviour, TTEntityId entity_id, void *fp
) {
    (void) behaviour;
    (void) entity_id;
    (void) fp;

    // This should never be called as call will never indicate that the
    // behaviour is still running.
    tt_assert(false);
}

static void tt_behaviour_fail_do_interrupt(
    TTBehaviour *behaviour, TTEntityId entity_id, void *fp
) {
    (void) behaviour;
    (void) entity_id;
    (void) fp;

    // This should never be called as call will never indicate that the
    // behaviour is still running.
    tt_assert(false);
}


static size_t tt_behaviour_fail_max_stack_size(TTBehaviour *behaviour) {
    (void) behaviour;

    return 0;
}


static void tt_behaviour_fail_free(TTBehaviour *behaviour) {
    free(behaviour);
}


TTBehaviour *tt_behaviour_fail(void) {
    TTBehaviour *behaviour = (TTBehaviour *) malloc(sizeof(TTBehaviour));
    tt_assert(behaviour != NULL);
    *behaviour = (TTBehaviour) {
        .do_call = tt_behaviour_fail_do_call,
        .do_resume = tt_behaviour_fail_do_resume,
        .do_interrupt = tt_behaviour_fail_do_interrupt,

        .frame_size = 0,
        .compute_max_stack_size = tt_behaviour_fail_max_stack_size,

        .free = tt_behaviour_fail_free
    };

    return behaviour;
}

