#include "tt-behaviour-harvest-target.hpp"

#include <cstddef>

#include "tt-behaviour.hpp"
#include "tt-error.hpp"


class TTBehaviourHarvestTarget : public TTBehaviour {
public:
    TTBehaviourResult do_call(TTEntityId, void *fp) final;
    TTBehaviourResult do_resume(TTEntityId, void *fp) final;
    void do_interrupt(TTEntityId, void *fp) final;

    std::size_t max_stack_size(void) final;
    std::size_t frame_size(void) final;
};


TTBehaviourResult TTBehaviourHarvestTarget::do_call(TTEntityId, void *) {
    // TODO
    return TTBehaviourResult::SUCCEEDED;
}

TTBehaviourResult TTBehaviourHarvestTarget::do_resume(TTEntityId, void *) {
    // TODO
    tt_assert(false);
}

void TTBehaviourHarvestTarget::do_interrupt(TTEntityId, void *) {
    return;
}


std::size_t TTBehaviourHarvestTarget::max_stack_size(void) {
    return 0;
}

std::size_t TTBehaviourHarvestTarget::frame_size() {
    return 0;
}


TTBehaviour *tt_behaviour_harvest_target(void) {
    return new TTBehaviourHarvestTarget();
}
