#include "tt-resource-wood-map.h"

#include <stdbool.h>

#include "tt-error.h"
#include "tt-spatial-index.h"

static struct TTResourceWoodMapState {
    bool initialised;

    TTSpatialIndex index;
} state = {.initialised = false};

void
tt_resource_wood_map_startup(void) {
    tt_assert(state.initialised == false);

    tt_spatial_index_init(&state.index);

    state.initialised = true;
}

void
tt_resource_wood_map_shutdown(void) {
    tt_assert(state.initialised == true);

    tt_spatial_index_destroy(&state.index);

    state.initialised = false;
}

TTSpatialIndex *
tt_resource_wood_map_get(void) {
    tt_assert(state.initialised == true);

    return &state.index;
}
