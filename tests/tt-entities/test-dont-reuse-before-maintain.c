/**
 * Checks that `tt_entities_release_id` won't make an id available for reuse too
 * soon.
 */
#include "tt-entities.h"

#include "tt-test.h"


int main(void) {
    tt_entities_startup();

    tt_assert(tt_entities_create() == 1);
    tt_assert(tt_entities_create() == 2);
    tt_assert(tt_entities_create() == 3);
    tt_entities_maintain();

    tt_entities_remove(2);
    tt_entities_maintain();

    tt_entities_remove(3);
    tt_assert(tt_entities_create() == 2);
    tt_assert(tt_entities_create() == 4);
    tt_entities_maintain();

    tt_assert(tt_entities_create() == 3);

    tt_entities_shutdown();

    return 0;
}

