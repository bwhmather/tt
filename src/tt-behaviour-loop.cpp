#include "tt-behaviour-loop.hpp"

#include <algorithm>
#include <cstdarg>
#include <cstddef>
#include <vector>

#include "tt-behaviour.hpp"
#include "tt-error.hpp"
#include "tt-entities.hpp"


class TTBehaviourLoop : public TTBehaviour {
    std::vector<TTBehaviour *> m_children;

public:
    TTBehaviourLoop(
        std::vector<TTBehaviour *> children
    ) : m_children(std::move(children)) {};

    TTBehaviourResult do_call(TTEntityId, void *fp) final;
    TTBehaviourResult do_resume(TTEntityId, void *fp) final;
    void do_interrupt(TTEntityId, void *fp) final;

    std::size_t max_stack_size(void) final;
    std::size_t frame_size(void) final;
};


struct TTBehaviourLoopState {
    size_t active_child;
};


TTBehaviourResult TTBehaviourLoop::do_call(
    TTEntityId entity_id, void *fp
) {
    TTBehaviourLoopState *state = (TTBehaviourLoopState *) fp;

    state->active_child = 0;
    while (true) {
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
        if (state->active_child >= this->m_children.size()) {
            state->active_child = 0;
        }
    }
}

TTBehaviourResult TTBehaviourLoop::do_resume(
    TTEntityId entity_id, void *fp
) {
    TTBehaviourLoopState *state = (TTBehaviourLoopState *) fp;

    TTBehaviour *active_child = this->m_children.at(state->active_child);
    TTBehaviourResult result = tt_behaviour_resume(active_child, entity_id, fp);

    while (true) {
        if (result == TTBehaviourResult::RUNNING) {
            return result;
        }
        if (result == TTBehaviourResult::FAILED) {
            return result;
        }

        state->active_child++;
        if (state->active_child >= this->m_children.size()) {
            state->active_child = 0;
        }

        TTBehaviour *active_child = this->m_children.at(state->active_child);
        result = tt_behaviour_call(active_child, entity_id, fp);
    }
}

void TTBehaviourLoop::do_interrupt(
    TTEntityId entity_id, void *fp
) {
    TTBehaviourLoopState *state = (TTBehaviourLoopState *) fp;

    TTBehaviour *active_child = this->m_children.at(state->active_child);
    tt_behaviour_interrupt(active_child, entity_id, fp);
}


std::size_t TTBehaviourLoop::max_stack_size(void) {
    std::size_t max_child_stack_size = 0;

    for (TTBehaviour *child : this->m_children) {
        max_child_stack_size = std::max(
            tt_behaviour_max_stack_size(child),
            max_child_stack_size
        );
    }

    return tt_behaviour_frame_size(this) + max_child_stack_size;

}

std::size_t TTBehaviourLoop::frame_size() {
    return sizeof(TTBehaviourLoopState);
}


TTBehaviour *tt_behaviour_loop(TTBehaviour *first, ...) {
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

    return new TTBehaviourLoop(children);
}


#define tt_behaviour_loop(...)                                              \
    tt_behaviour_loop_from_array((TTBehaviour *[]){__VA_ARGS__, NULL})

TTBehaviour *tt_behaviour_loop_from_array(TTBehaviour *children[]) {
    size_t num_children;
    for (num_children = 0; children[num_children] != NULL; num_children++) {}

    TTBehaviourLoop *behaviour = (TTBehaviourLoop *) malloc(
        sizeof(TTBehaviourLoop) +
        sizeof(TTBehaviour *) * (num_children + 1)
    );
    tt_assert(first != NULL);

    behaviour->behaviour = (TTBehaviour) {
        .on_call = tt_behaviour_loop_on_call;
        .on_resume = tt_behaviour_loop_on_resume;
        .on_interrupt = tt_behaviour_loop_on_interrupt;
    }

    behaviour->num_children = num_children;
    memcpy(
        &behaviour->children, children,
        num_children * sizeof(TTBehaviour *)
    );
}




