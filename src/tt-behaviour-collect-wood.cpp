#include "tt-behaviour-collect-wood.hpp"

#include "tt-behaviour.hpp"
#include "tt-behaviour-equip-axe.hpp"
#include "tt-behaviour-harvest-target.hpp"
#include "tt-behaviour-inventory-full.hpp"
#include "tt-behaviour-return sequence.hpp"
#include "tt-behaviour-selector.hpp"
#include "tt-behaviour-selector-until.hpp"
#include "tt-behaviour-select-stockpile.hpp"
#include "tt-behaviour-select-tree.hpp"
#include "tt-behaviour-select-wood-pile.hpp"
#include "tt-behaviour-sequence.hpp"
#include "tt-behaviour-walk-to-target.hpp"


TTBehaviour *tt_behaviour_collect_wood(void) {
    return tt_behaviour_sequence(
        tt_behaviour_selector_until(
            tt_behaviour_inventory_full(),

            // Get wood from existing bundle.
            tt_behaviour_sequence(
                tt_behaviour_select_wood_bundle(),
                tt_behaviour_walk_to_target(),
                tt_behaviour_harvest_target(),
                NULL
            )

            // Get wood by chopping down tree.
            tt_behaviour_sequence(
                tt_behaviour_selector(
                    tt_behaviour_equip_axe(),
                    NULL
                ),

                tt_behaviour_select_tree(),
                tt_behaviour_walk_to_target(),
                tt_behaviour_harvest_target(),
                NULL
            ),
            NULL
        ),

        // Drop wood at stockpile.
        tt_behaviour_select_stockpile(),
        tt_behaviour_walk_to_target(),
        NULL
    );
}
