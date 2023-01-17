#pragma once

#include "bt.h"

#define tt_behaviour_loop(...)                                                 \
    tt_behaviour_loop_from_array((BTBehaviour *[]){__VA_ARGS__, NULL})

BTBehaviour *
tt_behaviour_loop_from_array(BTBehaviour *children[]);
