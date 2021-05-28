#pragma once

#include <stddef.h>

#include "tt-entities.h"

typedef enum {
    TT_BEHAVIOUR_RUNNING,
    TT_BEHAVIOUR_SUCCEEDED,
    TT_BEHAVIOUR_FAILED
} TTBehaviourResult;

typedef struct TTBehaviour TTBehaviour;

struct TTBehaviour {
    TTBehaviourResult (*do_call)(TTBehaviour *, TTEntityId, void *fp);
    TTBehaviourResult (*do_resume)(TTBehaviour *, TTEntityId, void *fp);
    void (*do_interrupt)(TTBehaviour *, TTEntityId, void *fp);

    size_t frame_size;
    size_t (*compute_max_stack_size)(TTBehaviour *);

    void (*free)(TTBehaviour *);
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

size_t tt_behaviour_frame_size(TTBehaviour *behaviour);
size_t tt_behaviour_max_stack_size(TTBehaviour *behaviour);

void tt_behaviour_free(TTBehaviour *behaviour);
