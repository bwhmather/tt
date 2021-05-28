#pragma once
extern "C" {
#include "tt-entities.h"
}

void tt_system_untarget_dead_startup(void);
void tt_system_untarget_dead_shutdown(void);

void tt_system_untarget_dead_run(void);

