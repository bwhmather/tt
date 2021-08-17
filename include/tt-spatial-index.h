#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "tt-entities.h"

typedef struct {
    /* An arbitrary origin point on the line. */
    double origin_x;
    double origin_y;

    /* A unit vector from the origin, indicating its direction. */
    double direction_x;
    double direction_y;
} TTSpatialIndexLine;

typedef struct {
    uint64_t hash;
    TTEntityId entity_id;
} TTSpatialIndexEntry;

typedef struct {
    TTSpatialIndexLine lines[64];

    TTSpatialIndexEntry *entries;
    size_t entry_count;
    size_t entry_capacity;
} TTSpatialIndex;

void tt_spatial_index_init(TTSpatialIndex *index);
void tt_spatial_index_destroy(TTSpatialIndex *index);

void tt_spatial_index_clear(TTSpatialIndex *index);
void tt_spatial_index_add(
    TTSpatialIndex *index, TTEntityId entity_id, double x, double y
);
void tt_spatial_index_rebuild(TTSpatialIndex *index);

/**
 * Take a decent guess at the nearest entity to a point by bisecting the array
 * to find the hash with roughly the longest prefix of matching leading bits.
 */
TTEntityId tt_spatial_index_nearest(
    TTSpatialIndex *index, double x, double y
);
