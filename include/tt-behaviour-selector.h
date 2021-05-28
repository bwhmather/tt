#pragma once

#include "tt-behaviour.h"


#define tt_behaviour_selector(...)                                          \
    tt_behaviour_selector_from_array((TTBehaviour *[]){__VA_ARGS__, NULL})

TTBehaviour *tt_behaviour_selector_from_array(TTBehaviour *children[]);
