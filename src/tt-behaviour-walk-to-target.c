#include "tt-behaviour-walk-to-target.h"

#include <malloc.h>
#include <stddef.h>

#include "tt-behaviour.h"
#include "tt-error.h"


static TTBehaviourResult tt_behaviour_walk_to_target_do_call(
    TTBehaviour *behaviour, TTEntityId entity_id, void *fp
) {
    // TODO
    return TT_BEHAVIOUR_SUCCEEDED;
}

static TTBehaviourResult tt_behaviour_walk_to_target_do_resume(
    TTBehaviour *behaviour, TTEntityId entity_id, void *fp
) {
    // TODO
    tt_assert(false);
}

static void tt_behaviour_walk_to_target_do_interrupt(
    TTBehaviour *behaviour, TTEntityId entity_id, void *fp) {
    return;
}


static size_t tt_behaviour_walk_to_target_max_stack_size(TTBehaviour *behaviour) {
    (void *) behaviour;
    return 0;
}


static void tt_behaviour_walk_to_target_free(TTBehaviour *behaviour) {
    free(behaviour);
}


TTBehaviour *tt_behaviour_walk_to_target(void) {
    TTBehaviour *behaviour = (TTBehaviour *) malloc(sizeof(TTBehaviour));
    tt_assert(behaviour != NULL);
    *behaviour = (TTBehaviour) {
        .do_call = tt_behaviour_walk_to_target_do_call,
        .do_resume = tt_behaviour_walk_to_target_do_resume,
        .do_interrupt = tt_behaviour_walk_to_target_do_interrupt,

        .frame_size = 0,
        .compute_max_stack_size = tt_behaviour_walk_to_target_max_stack_size,

        .free = tt_behaviour_walk_to_target_free
    };

    return behaviour;
}

