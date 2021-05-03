#include "tt-entities.hpp"

#include "tt-test.hpp"


int main(void) {
    tt::entities::startup();

    tt::entities::create();
    tt::entities::create();
    tt::entities::create();
    tt::entities::create();
    tt::entities::create();

    tt::entities::remove(2);
    tt::entities::remove(5);

    tt::entities::maintain();

    tt::EntityIter iter;
    tt::entities::iter_begin(&iter);
    tt_assert(tt::entities::iter_has_next(&iter));
    tt_assert(tt::entities::iter_next(&iter) == 1);
    tt_assert(tt::entities::iter_has_next(&iter));
    tt_assert(tt::entities::iter_next(&iter) == 3);
    tt_assert(tt::entities::iter_has_next(&iter));
    tt_assert(tt::entities::iter_next(&iter) == 4);
    tt_assert(!tt::entities::iter_has_next(&iter));

    tt::entities::shutdown();

    return 0;
}

