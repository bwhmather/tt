#pragma once

#include "tt-types.h"

#include <stdbool.h>

void tt_entities_startup(void);
void tt_entities_shutdown(void);

TTEntityId tt_entities_new_id(void);
void tt_entities_release_id(TTEntityId entity_id);

int tt_entities_bind_release_callback(
    void (*callback) (TTEntityId id)
);
void tt_entities_unbind_release_callback(int handle);
