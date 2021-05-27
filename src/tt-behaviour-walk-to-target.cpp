#include "tt-behaviour-walk-to-target.hpp"

#include <cstddef>

#include "tt-behaviour.hpp"
#include "tt-error.hpp"


class TTBehaviourWalkToTarget : public TTBehaviour {
public:
    TTBehaviourResult do_call(TTEntityId, void *fp) final;
    TTBehaviourResult do_resume(TTEntityId, void *fp) final;
    void do_interrupt(TTEntityId, void *fp) final;

    std::size_t max_stack_size(void) final;
    std::size_t frame_size(void) final;
};


TTBehaviourResult TTBehaviourWalkToTarget::do_call(TTEntityId, void *) {
    // TODO
    return TTBehaviourResult::SUCCEEDED;
}

TTBehaviourResult TTBehaviourWalkToTarget::do_resume(TTEntityId, void *) {
    // TODO
    tt_assert(false);
}

void TTBehaviourWalkToTarget::do_interrupt(TTEntityId, void *) {
    return;
}


std::size_t TTBehaviourWalkToTarget::max_stack_size(void) {
    return 0;
}

std::size_t TTBehaviourWalkToTarget::frame_size() {
    return 0;
}


TTBehaviour *tt_behaviour_walk_to_target(void) {
    return new TTBehaviourWalkToTarget();
}
