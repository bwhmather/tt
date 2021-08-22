#include "tt-spatial-index.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "tt-entities.h"
#include "tt-error.h"


static uint64_t tt_spatial_index_hash(
    TTSpatialIndex *index, double x, double y
) {
    uint64_t hash = 0;

    for (int i = 0; i < 64; i++) {
        TTSpatialIndexLine *line = &index->lines[i];

        /* Vector from the line origin to the coordinate. */
        double offset_x = x - line->origin_x;
        double offset_y = y - line->origin_y;

        /* The projection of the vector from the line origin to the coordinate
         * onto the tangent of the line direction vector. */
        double tang_proj = (
            offset_x * line->direction_y - offset_y * line->direction_x
        );

        /* If negative then the coordinate is on one side of the line, if
         * positive it is on the other.  Doesn't really matter how we define it
         * so long as we are consistent. */
        if (tang_proj > 0) {
            hash |= 1;
        }

        hash = hash << 1;
    }

    return hash;
}


void tt_spatial_index_init(TTSpatialIndex *index) {
    tt_assert(index != NULL);

    /* Generate one random line for each bit, each intersecting with the game
     * space. */
    for (int i = 0; i < 64; i++) {
        TTSpatialIndexLine *line = &index->lines[i];

        /* TODO: bounds should be configurable */
        line->origin_x = 2 * ((double) rand()) / ((double) RAND_MAX) - 1;
        line->origin_y = 2 * ((double) rand()) / ((double) RAND_MAX) - 1;

        double direction = 2 * M_PI * ((double) rand() / (double) RAND_MAX);
        line->direction_x = sin(direction);
        line->direction_y = cos(direction);
    }

    index->entries = NULL;
    index->entry_count = 0;
    index->entry_capacity = 0;
}

void tt_spatial_index_destroy(TTSpatialIndex *index) {
    tt_assert(index != NULL);

    index->entry_count = 0;
    free(index->entries);
}


void tt_spatial_index_clear(TTSpatialIndex *index) {
    tt_assert(index != NULL);

    index->entry_count = 0;
}


void tt_spatial_index_add(
    TTSpatialIndex *index, TTEntityId entity_id, double x, double y
) {
    tt_assert(index != NULL);

    size_t entry_index = index->entry_count;

    index->entry_count += 1;
    if (index->entry_count > index->entry_capacity) {
        index->entries = realloc(
            index->entries, index->entry_count * sizeof(TTSpatialIndexEntry)
        );
        index->entry_capacity = index->entry_count;
    }

    TTSpatialIndexEntry *entry = &index->entries[entry_index];

    entry->hash = tt_spatial_index_hash(index, x, y);
    entry->entity_id = entity_id;
}


static int tt_spatial_index_compare_entries(
    const TTSpatialIndexEntry *a, const TTSpatialIndexEntry *b
) {
    if (a->hash < b->hash) return -1;
    if (a->hash > b->hash) return 1;
    return 0;
}


void tt_spatial_index_rebuild(TTSpatialIndex *index) {
    tt_assert(index != NULL);

    qsort(
        index->entries,
        index->entry_count,
        sizeof(TTSpatialIndexEntry),
        (int (*) (const void *, const void *)) tt_spatial_index_compare_entries
    );
}


TTEntityId tt_spatial_index_nearest(
    TTSpatialIndex *index, double x, double y
) {
    uint64_t hash = tt_spatial_index_hash(index, x, y);

    TTEntityId best_entity = 0;
    int best_quality = -1;

    for (size_t i = 0; i < index->entry_count; i++) {
        TTSpatialIndexEntry *entry = &index->entries[i];
        int quality = 64 - __builtin_popcountl(hash ^ entry->hash);

        if (quality > best_quality) {
            best_entity = entry->entity_id;
            best_quality = quality;
        }
    }

    return best_entity;
}
