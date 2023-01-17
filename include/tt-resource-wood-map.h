#pragma once

#include "tt-spatial-index.h"

void
tt_resource_wood_map_startup(void);
void
tt_resource_wood_map_shutdown(void);

TTSpatialIndex *
tt_resource_wood_map_get(void);
