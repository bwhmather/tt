#include "tt-storage-vector.h"

#include <random.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "tt-entities.h"
#include "tt-error.h"





typedef struct {
    uint64_t hash;
    TTEntityId entity;
} TTSpatialIndexEntry;



typedef struct {
    /* An arbitrary origin point on the line. */
    double origin_x;
    double origin_y;

    /* A unit vector from the origin, indicating its direction. */
    double direction_x;
    double direction_y;
} TTSpatialIndexLine;


typedef struct {
    TTSpatialIndexLine[64];
    TTSpatialIndexEntry *entries;,
} TTSpatialIndex;



/**
 * For each bit in the hash, generate a random line that intersects with the
 * world.  Presumably by generating a random point + a random direction.
 *
 * Allocate an array containing all entity ids and their spatial hash.
 *
 * Sort the array.
 *
 * Use bisect, seaching left and right, to yield the closest entities to a query.
 */



void tt_spatial_index_clear()
void tt_spatial_index_add(TTSpatialIndex *index, TTEntityId, entity_id, double x, double y);
void tt_spatial_index_rebuild(TTSpatialIndex *index);

TTEntityId tt_spatial_index_nearest(TTSpatialIndex *index, double x, double y);



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
            hash &= 1;
        }

        hash = hash << 1;
    }

    return hash;
}


TTSpatialIndex *tt_spatial_index_new(bounding_box) {
    TTSpatialIndex *index = (TTSpatialIndex *) malloc(sizeof(TTSpatialIndex));
    tt_assert(index != NULL);

    for (int i = 0; i < 64; i++) {
        TTSpatialIndexLine *line = index->lines[i];

        line->origin_x = 2 * ((double) rand()) / ((double) RAND_MAX) - 1;
        line->origin_y = 2 * ((double) rand()) / ((double) RAND_MAX) - 1;

        direction = 2 * M_PI * ((double) rand() / (double) RAND_MAX);
        line->direction_x = sin(direction);
        line->direction_y = cos(direction);
    }
}

