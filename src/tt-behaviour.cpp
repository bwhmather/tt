



typedef struct {
    TTBehaviourResult (*do_call) (TTBehaviour *behaviour, void *fp);
    TTBehaviourResult (*do_resume) (TTBehaviour *behaviour, void *fp);
    void (*do_interrupt) (TTBehaviour *behaviour, void *fp);
    void (*do_free)  (TTBehaviour *behaviour);

    size_t (*max_stack_size) (TTBehaviour *behaviour);
    size_t frame_size;


} TTBehaviour;






TTBehaviour *tt_behaviour_new() {

}


void tt_behaviour_free(TTBehaviour *behaviour) {
    behaviour->free_callback;
}



TTBehaviourResult tt_behaviour_call(
    TTBehaviour *behaviour, void *calling_fp
) {
    void *this_fp = calling_fp - behaviour->frame_size;
    return behaviour->do_call(behaviour, this_fp);
}


TTBehaviourResult tt_behaviour_resume(
    TTBehaviour *behaviour, void *calling_fp
) {
    void *this_fp = calling_fp - behaviour->frame_size;
    return behaviour->do_resume(behaviour, this_fp);
}


TTBehaviourResult tt_behaviour_interrupt(
    TTBehaviour *behaviour, void *calling_fp
) {
    void *this_fp = calling_fp - behaviour->frame_size;
    return behaviour->do_interrupt(behaviour, this_fp);
}

size_t tt_behaviour_frame_size() {

}

size_t tt_behaviour_max_stack_size() {

}

