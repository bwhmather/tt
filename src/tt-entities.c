#include "tt-entities.h"

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

static size_t tt_entities_max = 0;
static size_t tt_entities_free_list_length = 0;
static size_t tt_entities_free_list_capacity = 0;
static TTEntityId *tt_entities_free_list = NULL;


void tt_entities_startup(void) {
    assert(tt_entities_free_list == NULL);
    
    tt_entities_free_list_capacity = 256;
    tt_entities_free_list = (TTEntityId *) calloc(
        sizeof(TTEntityId), tt_entities_free_list_capacity
    );
}

void tt_entities_shutdown(void) {
    assert(tt_entities_free_list != NULL);

    free(tt_entities_free_list);
}

TTEntityId tt_entities_new_id(void) {
    assert(tt_entities_free_list != NULL);

    if (tt_entities_free_list_length) {
        tt_entities_free_list_length--;
        return tt_entities_free_list[tt_entities_free_list_length];
    } else {
        tt_entities_max++;
        return tt_entities_max;
    }
}

void tt_entities_release_id(TTEntityId entity_id) {
    assert(tt_entities_free_list != NULL);

    assert(entity_id != 0);
    assert(entity_id <= tt_entities_max);

    if (tt_entities_free_list_length == tt_entities_free_list_capacity) {
        tt_entities_free_list_capacity += tt_entities_free_list_capacity / 2;
        tt_entities_free_list = realloc(
            tt_entities_free_list,
            tt_entities_free_list_capacity * sizeof(TTEntityId)
        );
        assert(tt_entities_free_list != NULL);
    }

    tt_entities_free_list[tt_entities_free_list_length] = entity_id;
    tt_entities_free_list_length++;
}
