/**
 * Checks that `tt::entities::new_id` will return sequential ids.
 */
#include "tt-entities.hpp"

#include "tt-test.hpp"


int main(void) {
    tt::entities::startup();

    tt_assert(tt::entities::create() == 1);
    tt_assert(tt::entities::create() == 2);
    tt_assert(tt::entities::create() == 3);
    tt_assert(tt::entities::create() == 4);

    tt::entities::shutdown();

    return 0;
}

