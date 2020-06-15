#include "tt-entities.h"

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

static size_t tt_entities_max = 0;
static size_t tt_entities_num_free = 0;
static TTEntityId *tt_entities_free_list = NULL;


void tt_entities_startup(void) {
    assert(tt_entities_free_list == NULL);
    
    tt_entities_free_list = (TTEntityId *) calloc(sizeof(TTEntityId), 1024);
}

void tt_entities_shutdown(void) {
    assert(tt_entities_free_list != NULL);

    free(tt_entities_free_list);
}

TTEntityId tt_entities_new_id(void) {
    if (tt_entities_num_free) {
        tt_entities_num_free--;
        return tt_entities_free_list[tt_entities_num_free];
    } else {
        tt_entities_max++;
        return tt_entities_max;
    }
}

void tt_entities_release_id(TTEntityId entity_id) {
    assert(tt_entities_num_free < 1024);  // TODO realloc.
    tt_entities_free_list[tt_entities_num_free] = entity_id;
    tt_entities_num_free++;
}
