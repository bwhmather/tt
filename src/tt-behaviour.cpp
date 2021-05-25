#include "tt-behaviour.hpp"

#include <cstddef>

#include "tt-entities.hpp"


TTBehaviourResult tt_behaviour_call(
    TTBehaviour *behaviour, TTEntityId entity_id, void *calling_fp
) {
    void *this_fp = calling_fp - behaviour->frame_size();
    return behaviour->do_call(entity_id, this_fp);
}

TTBehaviourResult tt_behaviour_resume(
    TTBehaviour *behaviour, TTEntityId entity_id, void *calling_fp
) {
    void *this_fp = calling_fp - behaviour->frame_size();
    return behaviour->do_resume(entity_id, this_fp);
}

TTBehaviourResult tt_behaviour_interrupt(
    TTBehaviour *behaviour, TTEntityId entity_id, void *calling_fp
) {
    void *this_fp = calling_fp - behaviour->frame_size();
    behaviour->do_interrupt(entity_id, this_fp);
}


std::size_t tt_behaviour_frame_size(TTBehaviour *behaviour) {
    return behaviour->frame_size();
}

std::size_t tt_behaviour_max_stack_size(TTBehaviour *behaviour) {
    return behaviour->max_stack_size();
}

