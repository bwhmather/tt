#include "tt-system-behaviour.hpp"

#include <cassert>
#include <cmath>
#include <cstdio>

#include "tt-component-behaviour.hpp"
extern "C" {
#include "tt-behaviour.h"
#include "tt-entities.h"
#include "tt-error.h"
}

namespace state {
    static bool initialised = false;
}

void tt_system_behaviour_startup(void) {
    tt_assert(state::initialised == false);

    state::initialised = true;
}

void tt_system_behaviour_shutdown(void) {
    tt_assert(state::initialised == true);

    state::initialised = false;
}

void tt_system_behaviour_run(void) {
    TTEntityIter iter;

    tt_assert(state::initialised == true);

    tt_entities_iter_begin(&iter);

    while (tt_entities_iter_has_next(&iter)) {
        TTEntityId entity_id = tt_entities_iter_next(&iter);

        if (!tt_component_behaviour_has(entity_id)) continue;

        TTBehaviour *prev = tt_component_behaviour_get_prev(entity_id);
        TTBehaviour *next = tt_component_behaviour_get_next(entity_id);
        void *stack = tt_component_behaviour_get_stack(entity_id);

        if (prev != next && prev != NULL) {
            tt_behaviour_interrupt(prev, entity_id, stack);
        }

        if (next == NULL) {
            tt_component_behaviour_remove(entity_id);
        }

        TTBehaviourResult result;
        if (prev != next) {
            result = tt_behaviour_call(next, entity_id, stack);
        } else {
            result = tt_behaviour_resume(next, entity_id, stack);
        }

        if (result != TT_BEHAVIOUR_RUNNING) {
            next = NULL;
        }

        tt_component_behaviour_set_prev(entity_id, next);
        tt_component_behaviour_set_next(entity_id, next);
    }
}
