#include "tt-behaviour-select-tree.h"

#include <malloc.h>
#include <math.h>

#include "bt.h"
#include "tt-behaviour.h"
#include "tt-component-harvestable.h"
#include "tt-component-position.h"
#include "tt-component-target.h"
#include "tt-component-wood.h"
#include "tt-entities.h"
#include "tt-error.h"
#include "tt-utils.h"


static BTResult tt_behaviour_select_tree_tick(
    BTBehaviour *behaviour,
    void *state,
    TTBehaviourContext *context
) {
    (void) behaviour;
    (void) state;

    TTEntityId entity_id = context->entity_id;
    TTEntityIter iter;

    tt_entities_iter_begin(&iter);

    TTEntityId target_id = 0;
    double target_distance = INFINITY;

    while (tt_entities_iter_has_next(&iter)) {
        TTEntityId candidate_id = tt_entities_iter_next(&iter);

        if (!tt_component_harvestable_get(candidate_id)) continue;
        if (!tt_component_wood_has(candidate_id)) continue;
        if (!tt_component_position_has(candidate_id)) continue;

        double candidate_distance = tt_entity_distance(entity_id, candidate_id);

        if (candidate_distance < target_distance) {
            target_id = candidate_id;
            target_distance = candidate_distance;
        }
    }

    if (target_id) {
        tt_component_target_set(entity_id, target_id);
        return BT_SUCCEEDED;
    } else {
        return BT_FAILED;
    }
}


static void tt_behaviour_select_tree_free(BTBehaviour *behaviour) {
    free(behaviour);
}


BTBehaviour *tt_behaviour_select_tree(void) {
    BTBehaviour *behaviour = (BTBehaviour *) malloc(sizeof(BTBehaviour));
    tt_assert(behaviour != NULL);

    *behaviour = (BTBehaviour) {
        .init = NULL,
        .tick = (BTTickFn) tt_behaviour_select_tree_tick,
        .interrupt = NULL,

        .frame_size = 0,

        .free = tt_behaviour_select_tree_free
    };

    return behaviour;
}
