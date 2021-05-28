#pragma once

#include "tt-behaviour.h"


#define tt_behaviour_sequence(...)                                          \
    tt_behaviour_sequence_from_array((TTBehaviour *[]){__VA_ARGS__, NULL})

TTBehaviour *tt_behaviour_sequence_from_array(TTBehaviour *children[]);
