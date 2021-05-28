/**
 * Checks that `tt_entities_new_id` will return sequential ids.
 */
#include "tt-entities.h"

#include "tt-test.h"


int main(void) {
    tt_entities_startup();

    tt_assert(tt_entities_create() == 1);
    tt_assert(tt_entities_create() == 2);
    tt_assert(tt_entities_create() == 3);
    tt_assert(tt_entities_create() == 4);

    tt_entities_shutdown();

    return 0;
}

