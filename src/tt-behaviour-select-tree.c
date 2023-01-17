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
#include "tt-resource-wood-map.h"

static BTResult
tt_behaviour_select_tree_tick(
    BTBehaviour *behaviour, void *state, TTBehaviourContext *context
) {
    TTEntityId entity_id;
    TTEntityId tree_id;
    TTSpatialIndex *index;
    TTPosition *position;

    (void)behaviour;
    (void)state;

    entity_id = context->entity_id;
    if (!tt_component_position_has(entity_id))
        return BT_FAILED;
    position = tt_component_position_get(entity_id);

    index = tt_resource_wood_map_get();
    tree_id = tt_spatial_index_nearest(index, position->x, position->y);

    if (tree_id == 0)
        return BT_FAILED;
    if (!tt_component_harvestable_get(tree_id))
        return BT_FAILED;
    if (!tt_component_wood_has(tree_id))
        return BT_FAILED;
    if (!tt_component_position_has(tree_id))
        return BT_FAILED;

    tt_component_target_set(entity_id, tree_id);

    return BT_SUCCEEDED;
}

static void
tt_behaviour_select_tree_free(BTBehaviour *behaviour) {
    free(behaviour);
}

BTBehaviour *
tt_behaviour_select_tree(void) {
    BTBehaviour *behaviour = (BTBehaviour *)malloc(sizeof(BTBehaviour));
    tt_assert(behaviour != NULL);

    *behaviour = (BTBehaviour
    ){.init = NULL,
      .tick = (BTTickFn)tt_behaviour_select_tree_tick,
      .interrupt = NULL,

      .frame_size = 0,

      .free = tt_behaviour_select_tree_free};

    return behaviour;
}
