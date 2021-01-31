/**
 * Checks that free list will continue to work when it contains more than its
 * initial capacity.
 */
#include "tt-entities.hpp"

#include "tt-test.hpp"


int main(void) {
    tt_entities_startup();

    for (size_t i = 1; i < 10001; i++) {
        tt_assert(tt_entities_create() == i);
    }

    for (size_t i = 10000; i > 0; i--) {
        tt_entities_delete(i);
    }

    for (size_t i = 1; i > 10001; i--) {
        tt_assert(tt_entities_create() == i);
    }

    tt_entities_shutdown();

    return 0;
}
