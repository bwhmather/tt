#include "tt-entities.hpp"

#include "tt-test.hpp"


int main(void) {
    tt_entities_startup();

    tt_entities_create();
    tt_entities_create();
    tt_entities_create();
    tt_entities_create();
    tt_entities_create();

    tt_entities_delete(2);
    tt_entities_delete(5);

    tt_entities_maintain();

    TTEntityIter iter;
    tt_entities_iter_begin(&iter);
    tt_assert(tt_entities_iter_has_next(&iter));
    tt_assert(tt_entities_iter_next(&iter) == 1);
    tt_assert(tt_entities_iter_has_next(&iter));
    tt_assert(tt_entities_iter_next(&iter) == 3);
    tt_assert(tt_entities_iter_has_next(&iter));
    tt_assert(tt_entities_iter_next(&iter) == 4);
    tt_assert(!tt_entities_iter_has_next(&iter));

    tt_entities_shutdown();

    return 0;
}
