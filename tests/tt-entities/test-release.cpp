/**
 * Checks that `tt::entities::release_id` will make an id available for reuse.
 */
#include "tt-entities.hpp"

#include "tt-test.hpp"


int main(void) {
    tt::entities::startup();

    tt_assert(tt::entities::create() == 1);
    tt_assert(tt::entities::create() == 2);
    tt_assert(tt::entities::create() == 3);
    tt_assert(tt::entities::create() == 4);

    tt::entities::remove(2);
    tt_assert(tt::entities::create() == 5);
    tt_assert(tt::entities::create() == 6);

    tt::entities::remove(4);
    tt::entities::maintain();

    tt_assert(tt::entities::create() == 2);
    tt_assert(tt::entities::create() == 4);
    tt_assert(tt::entities::create() == 7);

    tt::entities::shutdown();

    return 0;
}

