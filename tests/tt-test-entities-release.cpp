/**
 * Checks that `tt_entities_release_id` will make an id available for reuse.
 */
#include "tt-entities.hpp"

#include "tt-test.hpp"


int main(void) {
    tt_entities_startup();

    tt_assert(tt_entities_new_id() == 1);
    tt_assert(tt_entities_new_id() == 2);
    tt_assert(tt_entities_new_id() == 3);
    tt_assert(tt_entities_new_id() == 4);

    tt_entities_release_id(2);
    tt_assert(tt_entities_new_id() == 2);
    tt_assert(tt_entities_new_id() == 5);

    tt_entities_shutdown();

    return 0;
}

