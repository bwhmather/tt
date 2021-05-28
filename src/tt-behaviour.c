#include "tt-behaviour.h"

#include <stddef.h>

#include "tt-entities.h"


static void *tt_behaviour_next_fp(TTBehaviour *behaviour, void *calling_fp) {
    return (void *) (((char *) calling_fp) - behaviour->frame_size);
}


TTBehaviourResult tt_behaviour_call(
    TTBehaviour *behaviour, TTEntityId entity_id, void *calling_fp
) {
    return behaviour->do_call(
        behaviour, entity_id, tt_behaviour_next_fp(behaviour, calling_fp)
    );
}

TTBehaviourResult tt_behaviour_resume(
    TTBehaviour *behaviour, TTEntityId entity_id, void *calling_fp
) {
    return behaviour->do_resume(
        behaviour, entity_id, tt_behaviour_next_fp(behaviour, calling_fp)
    );
}

void tt_behaviour_interrupt(
    TTBehaviour *behaviour, TTEntityId entity_id, void *calling_fp
) {
    behaviour->do_interrupt(
        behaviour, entity_id, tt_behaviour_next_fp(behaviour, calling_fp)
    );
}


size_t tt_behaviour_frame_size(TTBehaviour *behaviour) {
    return behaviour->frame_size;
}

size_t tt_behaviour_max_stack_size(TTBehaviour *behaviour) {
    return behaviour->compute_max_stack_size(behaviour);
}

