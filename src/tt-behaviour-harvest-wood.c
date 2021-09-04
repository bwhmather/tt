#include "tt-behaviour-harvest-wood.h"

#include <malloc.h>
#include <math.h>
#include <stddef.h>

#include "bt.h"
#include "tt-behaviour.h"
#include "tt-component-wood.h"
#include "tt-component-target.h"
#include "tt-component-harvestable.h"
#include "tt-error.h"
#include "tt-entities.h"


typedef struct {
    double readiness;
} TTBehaviourHarvestTargetState;


static void tt_behaviour_harvest_wood_init(
    BTBehaviour *behaviour,
    TTBehaviourHarvestTargetState *state,
    TTBehaviourContext *context
) {
    (void) behaviour;
    (void) context;

    state->readiness = 0.0;
}


static int BT_BEHAVIOUR_HARVEST_CAPACITY = 100;

static BTResult tt_behaviour_harvest_wood_tick(
    BTBehaviour *behaviour,
    TTBehaviourHarvestTargetState *state,
    TTBehaviourContext *context
) {
    (void) behaviour;

    TTEntityId entity_id = context->entity_id;

    // Check that we still need wood and are able to carry it.
    // TODO
    // if (carrying_something_other_than_wood()) {
    //     return BT_FAILED;
    // }
    if (
        tt_component_wood_has(entity_id) &&
        tt_component_wood_get(entity_id) >= BT_BEHAVIOUR_HARVEST_CAPACITY
    ) {
        return BT_SUCCEEDED;
    }

    // Check that the target exists and that we are able to harvest it.  No need
    // to check if it's empty, as empty trees will be killed.
    if (!tt_component_target_has(entity_id)) {
        return BT_FAILED;
    }
    TTEntityId target_id = tt_component_target_get(entity_id);

    if (!tt_component_harvestable_get(target_id)) {
        return BT_FAILED;
    }

    // Prepare for 3 seconds before taking a swing.
    if (state->readiness < 3.0) {
        state->readiness += 1.0 / 60.0;
        return BT_RUNNING;
    }
    state->readiness = 0.0;


    // Figure out how much we can get from the tree, and how much of it would
    // fit in our inventory.
    int entity_wood = tt_component_wood_has(entity_id)
        ? tt_component_wood_get(entity_id) : 0;
    int target_wood = tt_component_wood_has(target_id)
        ? tt_component_wood_get(target_id) : 0;


    int transferred_wood = 40;
    if (transferred_wood > target_wood) {
        transferred_wood = target_wood;
    }

    if (transferred_wood > BT_BEHAVIOUR_HARVEST_CAPACITY - entity_wood) {
        transferred_wood = BT_BEHAVIOUR_HARVEST_CAPACITY - entity_wood;
    }

    // Remove it from the tree.  Kill the tree if it no longer contains any
    // wood.
    target_wood -= transferred_wood;
    tt_component_wood_set(target_id, target_wood);
    if (target_wood <= 0) {
        tt_entities_remove(target_id);
    }

    // Add the harvested wood to the inventory.  Check once more to see if the
    // inventory is full.
    entity_wood += transferred_wood;
    tt_component_wood_set(entity_id, entity_wood);
    if (entity_wood >= BT_BEHAVIOUR_HARVEST_CAPACITY) {
        return BT_SUCCEEDED;
    }

    return BT_RUNNING;
}


static void tt_behaviour_harvest_wood_free(BTBehaviour *behaviour) {
    free(behaviour);
}


BTBehaviour *tt_behaviour_harvest_wood(void) {
    BTBehaviour *behaviour = (BTBehaviour *) malloc(sizeof(BTBehaviour));
    tt_assert(behaviour != NULL);

    *behaviour = (BTBehaviour) {
        .init = (BTInitFn) tt_behaviour_harvest_wood_init,
        .tick = (BTTickFn) tt_behaviour_harvest_wood_tick,
        .interrupt = NULL,

        .frame_size = sizeof(TTBehaviourHarvestTargetState),

        .free = tt_behaviour_harvest_wood_free
    };

    return behaviour;
}
