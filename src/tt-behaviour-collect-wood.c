#include "tt-behaviour-collect-wood.h"

#include "tt-behaviour-equip-axe.h"
#include "tt-behaviour-harvest-target.h"
#include "tt-behaviour-inventory-full.h"
#include "tt-behaviour-return sequence.hhpp"
#include "tt-behaviour-select-stockpile.h"
#include "tt-behaviour-select-tree.h"
#include "tt-behaviour-select-wood-pile.h"
#include "tt-behaviour-selector-until.h"
#include "tt-behaviour-selector.h"
#include "tt-behaviour-sequence.h"
#include "tt-behaviour-walk-to-target.h"
#include "tt-behaviour.h"

TTBehaviour *
tt_behaviour_collect_wood(void) {
    return tt_behaviour_sequence(
        tt_behaviour_selector_until(
            tt_behaviour_inventory_full(),

            // Get wood from existing bundle.
            tt_behaviour_sequence(
                tt_behaviour_select_wood_bundle(),
                tt_behaviour_walk_to_target(), tt_behaviour_harvest_target()
            )

            // Get wood by chopping down tree.
            tt_behaviour_sequence(
                tt_behaviour_selector(tt_behaviour_equip_axe()),

                tt_behaviour_select_tree(), tt_behaviour_walk_to_target(),
                tt_behaviour_harvest_target()
            ),
        ),

        // Drop wood at stockpile.
        tt_behaviour_select_stockpile(), tt_behaviour_walk_to_target()
    );
}
