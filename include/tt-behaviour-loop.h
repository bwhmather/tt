#pragma once

#include "tt-behaviour.h"


#define tt_behaviour_loop(...)                                              \
    tt_behaviour_loop_from_array((TTBehaviour *[]){__VA_ARGS__, NULL})

TTBehaviour *tt_behaviour_loop_from_array(TTBehaviour *children[]);
