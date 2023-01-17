#pragma once

#include "bt.h"

#define tt_behaviour_sequence(...)                                             \
    tt_behaviour_sequence_from_array((BTBehaviour *[]){__VA_ARGS__, NULL})

BTBehaviour *
tt_behaviour_sequence_from_array(BTBehaviour *children[]);
