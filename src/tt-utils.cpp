#include "tt-utils.hpp"

#include <cmath>
#include <limits>

#include "tt-component-position.hpp"
extern "C" {
#include "tt-entities.h"
}


double tt_entity_distance(TTEntityId a, TTEntityId b) {
    if (!tt_has_position(a) || !tt_has_position(b)) {
        return std::numeric_limits<double>::infinity();
    }

    TTPosition &pos_a = tt_get_position(a);
    TTPosition &pos_b = tt_get_position(b);

    return std::sqrt(
        std::pow(pos_a.x - pos_b.x, 2) +
        std::pow(pos_a.y - pos_b.y, 2)
    );
}
