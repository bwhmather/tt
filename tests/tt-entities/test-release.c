/**
 * Checks that `tt_entities_release_id` will make an id available for reuse.
 */
#include "tt-entities.h"

#include "tt-test.h"


int main(void) {
    tt_entities_startup();

    tt_assert(tt_entities_create() == 1);
    tt_assert(tt_entities_create() == 2);
    tt_assert(tt_entities_create() == 3);
    tt_assert(tt_entities_create() == 4);

    tt_entities_remove(2);
    tt_assert(tt_entities_create() == 5);
    tt_assert(tt_entities_create() == 6);

    tt_entities_remove(4);
    tt_entities_maintain();

    tt_assert(tt_entities_create() == 2);
    tt_assert(tt_entities_create() == 4);
    tt_assert(tt_entities_create() == 7);

    tt_entities_shutdown();

    return 0;
}

