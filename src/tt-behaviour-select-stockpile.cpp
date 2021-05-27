#include "tt-behaviour-select-stockpile.hpp"

#include <cstddef>

#include "tt-behaviour.hpp"
#include "tt-error.hpp"


class TTBehaviourSelectStockpile : public TTBehaviour {
public:
    TTBehaviourResult do_call(TTEntityId, void *fp) final;
    TTBehaviourResult do_resume(TTEntityId, void *fp) final;
    void do_interrupt(TTEntityId, void *fp) final;

    std::size_t max_stack_size(void) final;
    std::size_t frame_size(void) final;
};


TTBehaviourResult TTBehaviourSelectStockpile::do_call(TTEntityId, void *) {
    // TODO
    return TTBehaviourResult::SUCCEEDED;
}

TTBehaviourResult TTBehaviourSelectStockpile::do_resume(TTEntityId, void *) {
    // TODO
    tt_assert(false);
}

void TTBehaviourSelectStockpile::do_interrupt(TTEntityId, void *) {
    return;
}


std::size_t TTBehaviourSelectStockpile::max_stack_size(void) {
    return 0;
}

std::size_t TTBehaviourSelectStockpile::frame_size() {
    return 0;
}


TTBehaviour *tt_behaviour_select_stockpile(void) {
    return new TTBehaviourSelectStockpile();
}
