




typedef struct {
    TTBehaviour behaviour;
    size_t num_children;
    TTBehaviour *children[];
} TTBehaviourSequence;

typedef struct {
    size_t active_child;
} TTBehaviourSequenceState;





static TTBehaviourResult tt_behaviour_sequence_call(
    TTBehaviour *behaviour, void *fp
) {
    TTBehaviourSequence *sequence = (TTBehaviourSequence *) behaviour;
    TTBehaviourSequenceState *state = (TTBehaviourSequenceState *) fp;

    state->active_child = 0;
    while (state->active_child < sequence->num_children) {
        TTBehaviourResult result = tt_behaviour_call(state->active_child, fp);
        if (result == RUNNING || result == FAILED) {
            return result;
        }

        state->active_child++;
    }

    return SUCCEEDED;
}


static TTBehaviourResult tt_behaviour_sequence_resume(
    TTBehaviour *behaviour, void *fp
) {
    TTBehaviourSequence *sequence = (TTBehaviourSequence *) behaviour;
    TTBehaviourSequenceState *state = (TTBehaviourSequenceState *) fp;

    TTBehaviourResult result = tt_behaviour_resume(state->active_child, fp);
    while (state->active_child < sequence->num_children) {
        if (result == RUNNING || result == FAILED) {
            return result
        }

        state->active_child++;
        result = tt_behaviour_call(state->active_child, fp);
    }

    return SUCCEEDED;
}


static void tt_behaviour_sequence_interrupt(
    TTBehaviour *behaviour, void *fp
) {
    TTBehaviourSequence *sequence = (TTBehaviourSequence *) behaviour;
    TTBehaviourSequenceState *state = (TTBehaviourSequenceState *) fp;

    tt_behaviour_interrupt(state->active_child, fp);
}


TTBehaviour *tt_behaviour_sequence() {
    return tt_behaviour_new_va(


    );
}

