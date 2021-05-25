#include "tt-system-behaviour.hpp"

#include <cassert>
#include <cmath>
#include <cstdio>

#include "tt-component-behaviour.hpp"
#include "tt-entities.hpp"
#include "tt-error.hpp"

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

        TTBehaviourComponent component = tt_behaviour_component_get(entity_id);

        TTBehaviourResult result = tt_behaviour_run(
            component.behaviour, component.stack
        );

        if (result != RUNNING) {
            tt_component_behaviour_remove(entity_id);
        }
    }
}
