#pragma once

#include <cstddef>

enum class TTBehaviourResult {
    RUNNING,
    SUCCEEDED,
    FAILED
};

struct TTBehaviour {
    virtual ~TTBehaviour();

    virtual TTBehaviourResult do_call(void *fp);
    virtual TTBehaviourResult do_resume(void *fp);
    virtual void do_interrupt(void *fp);

    virtual std::size_t max_stack_size();
    virtual std::size_t frame_size();
};

TTBehaviourResult tt_behaviour_call(
    TTBehaviour *behaviour, void *calling_fp
);
TTBehaviourResult tt_behaviour_resume(
    TTBehaviour *behaviour, void *calling_fp
);
TTBehaviourResult tt_behaviour_interrupt(
    TTBehaviour *behaviour, void *calling_fp
);

std::size_t tt_behaviour_frame_size(TTBehaviour *behaviour);
std::size_t tt_behaviour_max_stack_size(TTBehaviour *behaviour);
