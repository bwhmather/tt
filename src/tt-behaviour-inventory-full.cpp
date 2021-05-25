#include "tt-behaviour-entity-full.hpp"

#include <cstddef>

#include "tt-behaviour.hpp"
#include "tt-errors.hpp"

class TTBehaviourInventoryFull : TTBehaviour {
    ~TTBehaviourInventoryFull();

    TTBehaviourResult do_call(TTEntityId, void *fp) final;
    TTBehaviourResult do_resume(TTEntityId, void *fp) final;
    void do_interrupt(TTEntityId, void *fp) final;

    std::size_t max_stack_size(void) final;
    std::size_t frame_size(void) final;
}



TTBehaviourResult TTBehaviourInventoryFull::do_call(TTEntityId, void *fp) {
    // TODO
    return TTBehaviourResult::SUCCEEDED;
}

TTBehaviourResult TTBehaviourInventoryFull::do_resume(TTEntityId, void *fp) {
    // TODO
    tt_assert(false);
}

void TTBehaviourInventoryFull::do_interrupt(TTEntityId, void *fp) {
    return;
}


std::size_t TTBehaviourInventoryFull::max_stack_size(void) {
    return 0;
}

std::size_t TTBehaviourInventoryFull::frame_size() {
    return 0;
}


TTBehaviour *tt_behaviour_inventory_full(void) {
    return new TTBehaviourInventoryFull();

}
