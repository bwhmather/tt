#pragma once

#include <cstddef>

#include "tt-entities.hpp"

enum class TTBehaviourResult {
    RUNNING,
    SUCCEEDED,
    FAILED
};

struct TTBehaviour {
    virtual ~TTBehaviour() {};

    virtual TTBehaviourResult do_call(TTEntityId, void *fp) = 0;
    virtual TTBehaviourResult do_resume(TTEntityId, void *fp) = 0;
    virtual void do_interrupt(TTEntityId, void *fp) = 0;

    virtual std::size_t max_stack_size() = 0;
    virtual std::size_t frame_size() = 0;
};

TTBehaviourResult tt_behaviour_call(
    TTBehaviour *behaviour, TTEntityId entity_id, void *calling_fp
);
TTBehaviourResult tt_behaviour_resume(
    TTBehaviour *behaviour, TTEntityId entity_id, void *calling_fp
);
void tt_behaviour_interrupt(
    TTBehaviour *behaviour, TTEntityId entity_id, void *calling_fp
);

std::size_t tt_behaviour_frame_size(TTBehaviour *behaviour);
std::size_t tt_behaviour_max_stack_size(TTBehaviour *behaviour);
