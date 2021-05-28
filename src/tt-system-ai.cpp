#include "tt-system-ai.hpp"

#include <cassert>
#include <cstdio>

extern "C" {
#include "tt-behaviour.h"
#include "tt-behaviour-harvest-target.h"
#include "tt-behaviour-inventory-full.h"
#include "tt-behaviour-loop.h"
#include "tt-behaviour-selector.h"
#include "tt-behaviour-selector-until.h"
#include "tt-behaviour-select-stockpile.h"
#include "tt-behaviour-select-tree.h"
#include "tt-behaviour-sequence.h"
#include "tt-behaviour-walk-to-target.h"
#include "tt-component-brain.h"
#include "tt-component-behaviour.h"
#include "tt-entities.h"
#include "tt-error.h"
}

namespace state {
    static bool initialised = false;

    TTBehaviour *collect_wood_behaviour;
    TTBehaviour *harvest_crops_behaviour;
    TTBehaviour *construct_buildings_behaviour;
    TTBehaviour *attack_behaviour;
    TTBehaviour *flee_behaviour;
}


void tt_system_ai_startup(void) {
    tt_assert(state::initialised == false);

    state::collect_wood_behaviour = tt_behaviour_sequence(
        tt_behaviour_loop(
            tt_behaviour_inventory_full(),

            // Get wood by chopping down tree.
            tt_behaviour_sequence(
                tt_behaviour_select_tree(),
                tt_behaviour_walk_to_target(),
                tt_behaviour_harvest_target(),
                NULL
            ),
            NULL
        ),

        // Drop wood at stockpile.
        tt_behaviour_select_stockpile(),
        tt_behaviour_walk_to_target(),
        NULL
    );
    state::harvest_crops_behaviour = NULL;
    state::construct_buildings_behaviour = NULL;
    state::attack_behaviour = NULL;
    state::flee_behaviour = NULL;

    state::initialised = true;
}

void tt_system_ai_shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::collect_wood_behaviour;
    delete state::harvest_crops_behaviour;
    delete state::construct_buildings_behaviour;
    delete state::attack_behaviour;
    delete state::flee_behaviour;

    state::initialised = false;
}


static float tt_ai_collect_wood_score(TTEntityId) {
    return 1.0f;
}

static float tt_ai_harvest_crops_score(TTEntityId) {
    return 0.0f;
}

static float tt_ai_construct_buildings_score(TTEntityId) {
    return 0.0f;
}

static float tt_ai_attack_score(TTEntityId) {
    return 0.0f;
}

static float tt_ai_flee_score(TTEntityId) {
    return 0.0f;
}

void tt_system_ai_run(void) {
    TTEntityIter iter;

    tt_assert(state::initialised == true);

    tt_entities_iter_begin(&iter);

    while (tt_entities_iter_has_next(&iter)) {
        TTEntityId entity_id = tt_entities_iter_next(&iter);

        if (!tt_component_brain_get(entity_id)) continue;

        TTBehaviour *new_behaviour = NULL;
        float best_score = 0.0f;

        float score;
        score = tt_ai_collect_wood_score(entity_id);
        if (score > best_score) {
            new_behaviour = state::collect_wood_behaviour;
            best_score = score;
        }

        score = tt_ai_harvest_crops_score(entity_id);
        if (score > best_score) {
            new_behaviour = state::harvest_crops_behaviour;
            best_score = score;
        }

        score = tt_ai_construct_buildings_score(entity_id);
        if (score > best_score) {
            new_behaviour = state::construct_buildings_behaviour;
            best_score = score;
        }

        score = tt_ai_attack_score(entity_id);
        if (score > best_score) {
            new_behaviour = state::attack_behaviour;
            best_score = score;
        }

        score = tt_ai_flee_score(entity_id);
        if (score > best_score) {
            new_behaviour = state::flee_behaviour;
            best_score = score;
        }

        tt_component_behaviour_set_next(entity_id, new_behaviour);
    }
}
