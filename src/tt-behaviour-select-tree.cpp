#include "tt-behaviour-select-tree.hpp"

#include <cstddef>

#include "tt-behaviour.hpp"
#include "tt-error.hpp"


class TTBehaviourSelectTree : public TTBehaviour {
public:
    TTBehaviourResult do_call(TTEntityId, void *fp) final;
    TTBehaviourResult do_resume(TTEntityId, void *fp) final;
    void do_interrupt(TTEntityId, void *fp) final;

    std::size_t max_stack_size(void) final;
    std::size_t frame_size(void) final;
};


TTBehaviourResult TTBehaviourSelectTree::do_call(TTEntityId, void *) {
    // TODO
    return TTBehaviourResult::SUCCEEDED;
}

TTBehaviourResult TTBehaviourSelectTree::do_resume(TTEntityId, void *) {
    // TODO
    tt_assert(false);
}

void TTBehaviourSelectTree::do_interrupt(TTEntityId, void *) {
    return;
}


std::size_t TTBehaviourSelectTree::max_stack_size(void) {
    return 0;
}

std::size_t TTBehaviourSelectTree::frame_size() {
    return 0;
}


TTBehaviour *tt_behaviour_select_tree(void) {
    return new TTBehaviourSelectTree();
}
