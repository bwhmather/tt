#include "tt-entities.h"

#include "tt-test.h"


int main(void) {
    tt_entities_startup();

    TTEntityIter iter;
    tt_entities_iter_begin(&iter);
    tt_assert(!tt_entities_iter_has_next(&iter));

    tt_entities_shutdown();

    return 0;
}

