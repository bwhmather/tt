#include "tt-behaviour-sequence.hpp"

#include <algorithm>
#include <cstdarg>
#include <cstddef>
#include <vector>

#include "tt-behaviour.hpp"
#include "tt-error.hpp"
#include "tt-entities.hpp"


class TTBehaviourSequence : public TTBehaviour {
    std::vector<TTBehaviour *> m_children;

public:
    TTBehaviourSequence(
        std::vector<TTBehaviour *> children
    ) : m_children(std::move(children)) {};

    TTBehaviourResult do_call(TTEntityId, void *fp) final;
    TTBehaviourResult do_resume(TTEntityId, void *fp) final;
    void do_interrupt(TTEntityId, void *fp) final;

    std::size_t max_stack_size(void) final;
    std::size_t frame_size(void) final;
};


struct TTBehaviourSequenceState {
    size_t active_child;
};


TTBehaviourResult TTBehaviourSequence::do_call(
    TTEntityId entity_id, void *fp
) {
    TTBehaviourSequenceState *state = (TTBehaviourSequenceState *) fp;

    state->active_child = 0;
    while (state->active_child < this->m_children.size()) {
        TTBehaviour *active_child = this->m_children.at(state->active_child);
        TTBehaviourResult result = tt_behaviour_call(
            active_child, entity_id, fp
        );
        if (result == TTBehaviourResult::RUNNING) {
            return result;
        }
        if (result == TTBehaviourResult::FAILED) {
            return result;
        }

        state->active_child++;
    }

    return TTBehaviourResult::SUCCEEDED;
}

TTBehaviourResult TTBehaviourSequence::do_resume(
    TTEntityId entity_id, void *fp
) {
    TTBehaviourSequenceState *state = (TTBehaviourSequenceState *) fp;

    TTBehaviour *active_child = this->m_children.at(state->active_child);
    TTBehaviourResult result = tt_behaviour_resume(active_child, entity_id, fp);
    while (state->active_child < this->m_children.size()) {
        if (result == TTBehaviourResult::RUNNING) {
            return result;
        }
        if (result == TTBehaviourResult::FAILED) {
            return result;
        }

        state->active_child++;
        TTBehaviour *active_child = this->m_children.at(state->active_child);
        result = tt_behaviour_call(active_child, entity_id, fp);
    }

    return TTBehaviourResult::SUCCEEDED;
}

void TTBehaviourSequence::do_interrupt(
    TTEntityId entity_id, void *fp
) {
    TTBehaviourSequenceState *state = (TTBehaviourSequenceState *) fp;

    TTBehaviour *active_child = this->m_children.at(state->active_child);
    tt_behaviour_interrupt(active_child, entity_id, fp);
}


std::size_t TTBehaviourSequence::max_stack_size(void) {
    std::size_t max_child_stack_size = 0;

    for (TTBehaviour *child : this->m_children) {
        max_child_stack_size = std::max(
            tt_behaviour_max_stack_size(child),
            max_child_stack_size
        );
    }

    return tt_behaviour_frame_size(this) + max_child_stack_size;

}

std::size_t TTBehaviourSequence::frame_size() {
    return sizeof(TTBehaviourSequenceState);
}


TTBehaviour *tt_behaviour_sequence(TTBehaviour *first, ...) {
    va_list args;
    va_start(args, first);

    std::vector<TTBehaviour *> children;

    for (
        TTBehaviour *child = first;
        child != NULL;
        child = va_arg(args, TTBehaviour *)
    ) {
        children.push_back(child);
    }

    va_end(args);

    return new TTBehaviourSequence(children);
}


