#include "tt-system-ai.hpp"

#include <cassert>
#include <cstdio>

#include "tt-component-position.hpp"
#include "tt-component-job.hpp"
#include "tt-entities.hpp"
#include "tt-error.hpp"

namespace state {
    static bool initialised = false;
}


typedef struct {
    TTBehaviourBeginFn *begin_fn;
    TTBehaviourInterruptFn *interrupt_fn;
    TTBehaviourTickFn *tick_fn;

    size_t stack_frame_size;

    size_t nchildren;
    TTBehaviour *children[];
} TTBehaviour;










float tt_ai_get_wood_score(TTEntityId) {


}



choose_goal {
    float score;

    TTGoal new_goal = TTGoal::NONE;
    best_score = 0.0f;

    score = tt_ai_collect_wood_score(entity_id)
    if (score > best_score) {
        new_goal = TTGoal::COLLECT_WOOD;
        best_score = score;
    }

    score = tt_ai_harvest_crops_score(entity_id)
    if (score > best_score) {
        new_goal = TTGoal::HARVEST_CROPS;
        best_score = score;
    }

    score = tt_ai_construct_building_score(entity_id)
    if (score > best_score) {
        new_goal = TTGoal::CONSTRUCT_BUILDING;
        best_score = score;
    }

    score = tt_ai_attack_score(entity_id)
    if (score > best_score) {
        new_goal = TTGoal::ATTACK;
        best_score = score;
    }

    score = tt_ai_flee_score(entity_id)
    if (score > best_score) {
        new_goal = TTGoal::FLEE;
        best_score = score;
    }
}



run() {

    for entity_id in entities {

    }

}







typedef struct {
    size_t active_child;
} TTBehaviorSelectorState;

void tt_behaviour_selector_begin(TTBehaviourState state) {
    reserve(sizeof(TTBehaviourSelectorState));



    tt_behavior_push(children[0]);
}

bool tt_behaviour_selector_interupt(TTBehaviorState state) {
    return tt_behaviour_interupt
}

TTBehaviourResult tt_behaviour_selector_tick(TTBehaviourState state) {

}


TTBehaviourResult tt_behaviour_open_door_tick(TTBehaviourState) {
    assert not children;


}



void tt_system_ai_startup(void) {
    tt_assert(state::initialised == false);


    // Enter door.
    begin_sequence();
        push_behaviour(walk_to_door);
        begin_selector();
            push_behaviour(open_door);
            begin_sequence();
                push_behaviour(unlock_door);
                push_behaviour(open_door);
            end_sequence();
        push_behaviour(smash_door);
        end_selector();
        push_behaviour(walk_through_door);
        push_behaviour(close_door);
    end_sequence();











    state::initialised = true;
}

void tt_system_ai_shutdown(void) {
    tt_assert(state::initialised == true);
    state::initialised = false;
}




















void tt_system_ai_run(void) {
    TTEntityIter iter;

    tt_assert(state::initialised == true);

    tt_entities_iter_begin(&iter);

    while (tt_entities_iter_has_next(&iter)) {
        TTEntityId entity_id = tt_entities_iter_next(&iter);

        TTGoal old_goal = tt_component_goal_get(entity_id);
        TTGoal new_goal = tt_system_ai_choose_goal(entity_id);

        if (new_goal != old_goal) {
            tt_behaviour_interrupt(entity_id);
            tt_component_goal_set(new_goal);
        }

        tt_behaviour_run(entity_id);




    }
}




