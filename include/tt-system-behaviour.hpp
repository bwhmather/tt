#pragma once
extern "C" {
#include "tt-entities.h"
}

void tt_system_behaviour_startup(void);
void tt_system_behaviour_shutdown(void);

void tt_system_behaviour_run(void);
