#include "tt-system-update-wood-map.h"

#include <stdbool.h>

#include "tt-component-harvestable.h"
#include "tt-component-position.h"
#include "tt-component-wood.h"
#include "tt-entities.h"
#include "tt-error.h"
#include "tt-resource-wood-map.h"


static struct TTSystemSpritesState {
    bool initialised;
} state = { .initialised = false };

void tt_system_update_wood_map_startup(void) {
    tt_assert(state.initialised == false);
    state.initialised = true;
}

void tt_system_update_wood_map_shutdown(void) {
    tt_assert(state.initialised == true);
    state.initialised = false;
}

void tt_system_update_wood_map_run(void) {
    TTEntityIter iter;
    TTSpatialIndex *index;

    tt_assert(state.initialised == true);

    index = tt_resource_wood_map_get();
    tt_spatial_index_clear(index);

    tt_entities_iter_begin(&iter);

    while (tt_entities_iter_has_next(&iter)) {
        TTEntityId entity_id = tt_entities_iter_next(&iter);

        if (!tt_component_wood_has(entity_id)) continue;
        if (!tt_component_harvestable_get(entity_id)) continue;
        if (!tt_component_position_has(entity_id)) continue;

        TTPosition *position = tt_component_position_get(entity_id);

        tt_spatial_index_add(index, entity_id, position->x, position->y);
    }

    tt_spatial_index_rebuild(index);
}
