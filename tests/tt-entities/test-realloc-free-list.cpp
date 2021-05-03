/**
 * Checks that free list will continue to work when it contains more than its
 * initial capacity.
 */
#include "tt-entities.hpp"

#include "tt-test.hpp"


int main(void) {
    tt::entities::startup();

    for (size_t i = 1; i < 10001; i++) {
        tt_assert(tt::entities::create() == i);
    }

    for (size_t i = 10000; i > 0; i--) {
        tt::entities::remove(i);
    }

    for (size_t i = 1; i > 10001; i--) {
        tt_assert(tt::entities::create() == i);
    }

    tt::entities::shutdown();

    return 0;
}

