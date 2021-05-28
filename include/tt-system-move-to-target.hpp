#pragma once
extern "C" {
#include "tt-entities.h"
}

void tt_system_move_to_target_startup(void);
void tt_system_move_to_target_shutdown(void);

void tt_system_move_to_target_run(void);
