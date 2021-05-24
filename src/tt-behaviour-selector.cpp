
typedef struct {
    size_t active_child;
} TTBehaviourSelectorState;


static void tt_behaviour_selector_begin() {

}

static TTBehaviourState tt_behaviour_selector_tick() {
    while (true) {
        if (state->active_child > tt_behaviour_num_children()) {
            return TTBehaviourState::SUCCEEDED;
        }

        TTBehaviourState result = tt_behaviour_child(state->active_child);
        tt

    }
}

static void tt_behaviour_selector_interrupt() {

}




TTBehaviour *tt_behaviour_selector() {

}

