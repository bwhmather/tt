




typedef struct {
    TTBehaviour behaviour;
    size_t num_children;
    TTBehaviour *children[];
} TTBehaviourSelector;

typedef struct {
    size_t active_child;
} TTBehaviourSelectorState;





static TTBehaviourResult tt_behaviour_selector_call(
    TTBehaviour *behaviour, void *fp
) {
    TTBehaviourSelector *selector = (TTBehaviourSelector *) behaviour;
    TTBehaviourSelectorState *state = (TTBehaviourSelectorState *) fp;

    state->active_child = 0;
    while (state->active_child < selector->num_children) {
        TTBehaviourResult result = tt_behaviour_call(state->active_child, fp);
        if (result == RUNNING || result == SUCCEEDED) {
            return result;
        }

        state->active_child++;
    }

    return FAILED;
}


static TTBehaviourResult tt_behaviour_selector_resume(
    TTBehaviour *behaviour, void *fp
) {
    TTBehaviourSelector *selector = (TTBehaviourSelector *) behaviour;
    TTBehaviourSelectorState *state = (TTBehaviourSelectorState *) fp;

    TTBehaviourResult result = tt_behaviour_resume(state->active_child, fp);
    while (state->active_child < selector->num_children) {
        if (result == RUNNING || result == SUCCEEDED) {
            return result
        }

        state->active_child++;
        result = tt_behaviour_call(state->active_child, fp);
    }

    return FAILED;
}


static void tt_behaviour_selector_interrupt(
    TTBehaviour *behaviour, void *fp
) {
    TTBehaviourSelector *selector = (TTBehaviourSelector *) behaviour;
    TTBehaviourSelectorState *state = (TTBehaviourSelectorState *) fp;

    tt_behaviour_interrupt(state->active_child, fp);
}


TTBehaviour *tt_behaviour_selector() {
    return tt_behaviour_new_va(


    );
}
