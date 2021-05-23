#include "tt-system-ai.hpp"

#include <cassert>
#include <cstdio>

#include "tt-component-position.hpp"
#include "tt-component-job.hpp"
#include "tt-entities.hpp"
#include "tt-error.hpp"

namespace state {
    static bool initialised = false;
}

void tt_system_ai_startup(void) {
    tt_assert(state::initialised == false);
    state::initialised = true;
}

void tt_system_ai_shutdown(void) {
    tt_assert(state::initialised == true);
    state::initialised = false;
}

void tt_system_ai_run(void) {
    TTEntityIter iter;

    tt_assert(state::initialised == true);

    tt_entities_iter_begin(&iter);

    while (tt_entities_iter_has_next(&iter)) {
        TTVertex vertex;
        TTEntityId entity_id = tt_entities_iter_next(&iter);

        if (!tt_has_position(entity_id)) continue;
        if (tt_has_job(entity_id) continue;

        if (tt_has_wood(entity_id) && tt_can

        if (tt_can_chop_wood(entity_id) {
            if (tt_has_wood(entity_id) && tt_get_wood(entity_id) > 0) {
                // Find stockpile.
                tt_set_target(entity_id, stockpile_entity_id);
                tt_set_job(transfer_resources);
            } else {
                // Find tree.
                tt_set_target(entity_id, tree_entity_id);
                tt_set_job(chop_wood);
            }
        }
    }
}




