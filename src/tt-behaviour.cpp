#include "tt-behaviour.hpp"

#include <cstddef>


TTBehaviourResult tt_behaviour_call(
    TTBehaviour *behaviour, void *calling_fp
) {
    void *this_fp = calling_fp - behaviour->frame_size();
    return behaviour->do_call(this_fp);
}

TTBehaviourResult tt_behaviour_resume(
    TTBehaviour *behaviour, void *calling_fp
) {
    void *this_fp = calling_fp - behaviour->frame_size();
    return behaviour->do_resume(this_fp);
}

TTBehaviourResult tt_behaviour_interrupt(
    TTBehaviour *behaviour, void *calling_fp
) {
    void *this_fp = calling_fp - behaviour->frame_size();
    behaviour->do_interrupt(this_fp);
}


std::size_t tt_behaviour_frame_size(TTBehaviour *behaviour) {
    return behaviour->frame_size();
}

std::size_t tt_behaviour_max_stack_size(TTBehaviour *behaviour) {
    return behaviour->max_stack_size();
}

