#include "tt-behaviour-succeed.h"

#include <malloc.h>
#include <stddef.h>

#include "tt-behaviour.h"
#include "tt-error.h"


static TTBehaviourResult tt_behaviour_succeed_do_call(
    TTBehaviour *behaviour, TTEntityId entity_id, void *fp
) {
    (void) behaviour;
    (void) entity_id;
    (void) fp;

    return TT_BEHAVIOUR_SUCCEEDED;
}

static TTBehaviourResult tt_behaviour_succeed_do_resume(
    TTBehaviour *behaviour, TTEntityId entity_id, void *fp
) {
    (void) behaviour;
    (void) entity_id;
    (void) fp;

    // This should never be called as call will never indicate that the
    // behaviour is still running.
    tt_assert(false);
}

static void tt_behaviour_succeed_do_interrupt(
    TTBehaviour *behaviour, TTEntityId entity_id, void *fp
) {
    (void) behaviour;
    (void) entity_id;
    (void) fp;

    // This should never be called as call will never indicate that the
    // behaviour is still running.
    tt_assert(false);
}


static size_t tt_behaviour_succeed_max_stack_size(TTBehaviour *behaviour) {
    (void) behaviour;

    return 0;
}


static void tt_behaviour_succeed_free(TTBehaviour *behaviour) {
    free(behaviour);
}


TTBehaviour *tt_behaviour_succeed(void) {
    TTBehaviour *behaviour = (TTBehaviour *) malloc(sizeof(TTBehaviour));
    tt_assert(behaviour != NULL);
    *behaviour = (TTBehaviour) {
        .do_call = tt_behaviour_succeed_do_call,
        .do_resume = tt_behaviour_succeed_do_resume,
        .do_interrupt = tt_behaviour_succeed_do_interrupt,

        .frame_size = 0,
        .compute_max_stack_size = tt_behaviour_succeed_max_stack_size,

        .free = tt_behaviour_succeed_free
    };

    return behaviour;
}

