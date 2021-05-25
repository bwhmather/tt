#include "tt-behaviour-selector.hpp"

#include <cstddef>
#include <vector>

#include "tt-behaviour.hpp"
#include "tt-error.hpp"
#include "tt-entities.hpp"


class TTBehaviourSelector : TTBehaviour {

    std::vector<TTBehaviour *> children;

public:
    ~TTBehaviourSelector();

    TTBehaviourResult do_call(TTEntityId, void *fp) final;
    TTBehaviourResult do_resume(TTEntityId, void *fp) final;
    void do_interrupt(TTEntityId, void *fp) final;

    std::size_t max_stack_size(void) final;
    std::size_t frame_size(void) final;
};


struct TTBehaviourSelectorState {
    size_t active_child;
};


TTBehaviourResult TTBehaviourSelector::do_call(
    TTEntityId entity_id, void *fp
) {
    TTBehaviourSelectorState *state = (TTBehaviourSelectorState *) fp;

    state->active_child = 0;
    while (state->active_child < this->num_children) {
        TTBehaviourResult result = tt_behaviour_call(state->active_child, fp);
        if (result == TTBehaviourResult::RUNNING) {
            return result;
        }
        if (result == TTBehaviourResult::SUCCEEDED) {
            return result;
        }

        state->active_child++;
    }

    return TTBehaviourResult::FAILED;
}


TTBehaviourResult TTBehaviourSelector::do_resume(
    TTEntityId *entity_id, void *fp
) {
    TTBehaviourSelectorState *state = (TTBehaviourSelectorState *) fp;

    TTBehaviourResult result = tt_behaviour_resume(state->active_child, fp);
    while (state->active_child < this->num_children) {
        if (result == TTBehaviourResult::RUNNING) {
            return result;
        }
        if (result == TTBehaviourResult::SUCCEEDED) {
            return result;
        }

        state->active_child++;
        result = tt_behaviour_call(state->active_child, fp);
    }

    return TTBehaviourResult::FAILED;
}


TTBehaviourResult TTBehaviourSelector::do_interrupt(
    TTEntityId entity_id, void *fp
) {
    TTBehaviourSelectorState *state = (TTBehaviourSelectorState *) fp;

    tt_behaviour_interrupt(state->active_child, fp);
}


TTBehaviour *tt_behaviour_selector(TTBehaviour *first, ...) {
    return tt_behaviour_new_va(


    );
}
