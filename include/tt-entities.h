#include "tt-types.h"

void tt_entities_startup(void);
void tt_entities_shutdown(void);

TTEntityId tt_entities_new_id(void);
void tt_entities_release_id(TTEntityId entity_id);

