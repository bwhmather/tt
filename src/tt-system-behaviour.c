#include "tt-system-behaviour.h"

#include "bt.h"
#include "tt-behaviour.h"
#include "tt-component-behaviour.h"
#include "tt-component-behaviour-stack.h"
#include "tt-entities.h"
#include "tt-error.h"


static struct TTSystemBehaviourState {
    bool initialised;
} state = { .initialised = false };


void tt_system_behaviour_startup(void) {
    tt_assert(state.initialised == false);

    state.initialised = true;
}

void tt_system_behaviour_shutdown(void) {
    tt_assert(state.initialised == true);

    state.initialised = false;
}

void tt_system_behaviour_run(void) {
    TTEntityIter iter;

    tt_assert(state.initialised == true);

    tt_entities_iter_begin(&iter);

    while (tt_entities_iter_has_next(&iter)) {
        TTEntityId entity_id = tt_entities_iter_next(&iter);

        if (tt_component_behaviour_has(entity_id)) {
            if (!tt_component_behaviour_stack_has(entity_id)) {
                tt_component_behaviour_stack_init(entity_id);
            }

            BTBehaviour *behaviour = tt_component_behaviour_get(entity_id);
            BTContext *stack = tt_component_behaviour_stack_get(entity_id);

            TTBehaviourContext context = {
                .entity_id=entity_id,
            };

            BTResult result = bt_run(behaviour, stack, &context);

            if (result != BT_RUNNING) {
                tt_component_behaviour_remove(entity_id);
            }
        }

        if (
            tt_component_behaviour_stack_has(entity_id) &&
            !tt_component_behaviour_has(entity_id)
        ) {
            tt_component_behaviour_stack_remove(entity_id);
        }
    }
}
