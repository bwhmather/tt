#include "tt-utils.h"

#include <math.h>

#include "tt-component-position.h"
#include "tt-entities.h"

double
tt_entity_distance(TTEntityId a, TTEntityId b) {
    if (!tt_component_position_has(a) || !tt_component_position_has(b)) {
        return INFINITY;
    }

    TTPosition *pos_a = tt_component_position_get(a);
    TTPosition *pos_b = tt_component_position_get(b);

    return sqrt(pow(pos_a->x - pos_b->x, 2) + pow(pos_a->y - pos_b->y, 2));
}
