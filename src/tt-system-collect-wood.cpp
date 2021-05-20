#include "tt-renderer.hpp"

#include <cassert>
#include <cstdio>
#include <limits>

#include "tt-component-harvestable.hpp"
#include "tt-component-job.hpp"
#include "tt-component-move-to-target.hpp"
#include "tt-component-position.hpp"
#include "tt-component-target.hpp"
#include "tt-entities.hpp"
#include "tt-utils.hpp"


static bool is_tree(TTEntityId entity_id) {
    if (!tt_is_harvestable(entity_id)) return false;
    if (!tt_has_wood(entity_id)) return false;
    return true;
}

static TTEntityId select_tree(TTEntityId entity_id) {
    if (tt_has_target(entity_id) && is_tree(tt_get_target(entity_id))) {
        return tt_get_target(entity_id);
    }

    // TODO
    TTEntityId nearest_id = 0;
    double nearest_distance = std::numeric_limits<double>::infinity();

    TTEntityIter iter;
    tt_entities_iter_begin(&iter);
    while (tt_entities_iter_has_next(&iter)) {
        TTEntityId candidate_id = tt_entities_iter_next(&iter);

        if (!is_tree(candidate_id)) continue;

        double candidate_distance = tt_entity_distance(
            entity_id, candidate_id
        );

        if (candidate_distance < nearest_distance) {
            nearest_id = candidate_id;
            nearest_distance = candidate_distance;
        }
    }

    return nearest_id;
}

static bool is_stockpile(TTEntityId entity_id) {
    // TODO
    return true;
}

static TTEntityId select_stockpile(TTEntityId entity_id) {
    if (tt_has_target(entity_id) && is_stockpile(tt_get_target(entity_id))) {
        return tt_get_target(entity_id);
    }

    // TODO
    TTEntityId nearest_id = 0;
    double nearest_distance = std::numeric_limits<double>::infinity();

    TTEntityIter iter;
    tt_entities_iter_begin(&iter);
    while (tt_entities_iter_has_next(&iter)) {
        TTEntityId candidate_id = tt_entities_iter_next(&iter);

        if (!is_stockpile(candidate_id)) continue;

        double candidate_distance = tt_entity_distance(
            entity_id, candidate_id
        );

        if (candidate_distance < nearest_distance) {
            nearest_id = candidate_id;
            nearest_distance = candidate_distance;
        }
    }

    return nearest_id;
}


void tt_system_collect_wood_run(void) {
    TTEntityIter iter;

    tt_entities_iter_begin(&iter);

    while (tt_entities_iter_has_next(&iter)) {
        TTVertex vertex;
        TTEntityId entity_id = tt_entities_iter_next(&iter);

        if (!tt_has_job(entity_id)) continue;
        if (tt_get_job(entity_id) != collect_wood) continue;


        // Get rid of current stash (TODO).

        // Collect axe (TODO).

        if (tt_get_wood(entity_id) < 30) {
            TTEntityId tree_id = select_tree(entity_id);
            tt_set_target(entity_id, tree_id);

            if (tt_entity_distance(entity_id, tree_id) > min_distance) {
                tt_set_move_to_target(entity_id, tree_id, min_distance);
            } else {
                int transferred = std::min(
                    30 - tt_get_wood(entity_id), tt_get_wood(tree_id)
                );

                tt_set_wood(entity_id, tt_get_wood(entity_id) + transferred);
                tt_set_wood(tree_id, tt_get_wood(tree_id) - transferred);

                if (!tt_has_wood(tree_id)) {
                    tt_entities_delete(tree_id);
                }
            }
        } else {
            TTEntityId stockpile_id = select_stockpile(entity_id);

            if (tt_entity_distance(entity_id, tree_id) > min_distance) {
                tt_set_move_to_target(entity_id, tree_id, min_distance);
            } else {
                transfer_resources(entity_id, stockpile_id);
            }
        }
    }
}

