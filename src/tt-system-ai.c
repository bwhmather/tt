#include "tt-system-ai.h"

#include "bt.h"
#include "tt-behaviour-harvest-target.h"
#include "tt-behaviour-idle.h"
#include "tt-behaviour-inventory-full.h"
#include "tt-behaviour-loop.h"
#include "tt-behaviour-selector.h"
#include "tt-behaviour-selector-until.h"
#include "tt-behaviour-select-stockpile.h"
#include "tt-behaviour-select-tree.h"
#include "tt-behaviour-sequence.h"
#include "tt-behaviour-succeed.h"
#include "tt-behaviour-walk-to-target.h"
#include "tt-component-brain.h"
#include "tt-component-behaviour.h"
#include "tt-entities.h"
#include "tt-error.h"


static struct TTSystemAIState {
    bool initialised;

    BTBehaviour *idle_behaviour;
    BTBehaviour *collect_wood_behaviour;
    BTBehaviour *harvest_crops_behaviour;
    BTBehaviour *construct_buildings_behaviour;
    BTBehaviour *attack_behaviour;
    BTBehaviour *flee_behaviour;
} state = { .initialised = false };


void tt_system_ai_startup(void) {
    tt_assert(state.initialised == false);

    state.idle_behaviour = tt_behaviour_loop(tt_behaviour_idle());

    state.collect_wood_behaviour = tt_behaviour_sequence(
        tt_behaviour_loop(
            //tt_behaviour_inventory_full(),

            // Get wood by chopping down tree.
            tt_behaviour_sequence(
                tt_behaviour_select_tree(),
                tt_behaviour_walk_to_target()
                //tt_behaviour_harvest_target()
            )
        ),
        tt_behaviour_loop(tt_behaviour_idle())

        // Drop wood at stockpile.
        //tt_behaviour_select_stockpile(),
        //tt_behaviour_walk_to_target()
    );
    state.harvest_crops_behaviour = tt_behaviour_succeed();
    state.construct_buildings_behaviour = tt_behaviour_succeed();
    state.attack_behaviour = tt_behaviour_succeed();
    state.flee_behaviour = tt_behaviour_succeed();

    state.initialised = true;
}

void tt_system_ai_shutdown(void) {
    tt_assert(state.initialised == true);

    bt_behaviour_free(state.idle_behaviour);
    bt_behaviour_free(state.collect_wood_behaviour);
    bt_behaviour_free(state.harvest_crops_behaviour);
    bt_behaviour_free(state.construct_buildings_behaviour);
    bt_behaviour_free(state.attack_behaviour);
    bt_behaviour_free(state.flee_behaviour);

    state.initialised = false;
}

static float tt_ai_idle_score(TTEntityId entity_id) {
    (void) entity_id;

    return 0.1f;
}

static float tt_ai_collect_wood_score(TTEntityId entity_id) {
    return 0.2f;
}

static float tt_ai_harvest_crops_score(TTEntityId entity_id) {
    return 0.0f;
}

static float tt_ai_construct_buildings_score(TTEntityId entity_id) {
    return 0.0f;
}

static float tt_ai_attack_score(TTEntityId entity_id) {
    return 0.0f;
}

static float tt_ai_flee_score(TTEntityId entity_id) {
    return 0.0f;
}

void tt_system_ai_run(void) {
    TTEntityIter iter;

    tt_assert(state.initialised == true);

    tt_entities_iter_begin(&iter);

    while (tt_entities_iter_has_next(&iter)) {
        TTEntityId entity_id = tt_entities_iter_next(&iter);

        if (!tt_component_brain_get(entity_id)) continue;

        BTBehaviour *new_behaviour = NULL;
        float best_score = 0.0f;

        float score;
        score = tt_ai_idle_score(entity_id);
        if (score > best_score) {
            new_behaviour = state.idle_behaviour;
            best_score = score;
        }

        score = tt_ai_collect_wood_score(entity_id);
        if (score > best_score) {
            new_behaviour = state.collect_wood_behaviour;
            best_score = score;
        }

        score = tt_ai_harvest_crops_score(entity_id);
        if (score > best_score) {
            new_behaviour = state.harvest_crops_behaviour;
            best_score = score;
        }

        score = tt_ai_construct_buildings_score(entity_id);
        if (score > best_score) {
            new_behaviour = state.construct_buildings_behaviour;
            best_score = score;
        }

        score = tt_ai_attack_score(entity_id);
        if (score > best_score) {
            new_behaviour = state.attack_behaviour;
            best_score = score;
        }

        score = tt_ai_flee_score(entity_id);
        if (score > best_score) {
            new_behaviour = state.flee_behaviour;
            best_score = score;
        }

        tt_component_behaviour_set(entity_id, new_behaviour);
    }
}
