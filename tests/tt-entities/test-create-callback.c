/**
 * Checks that create callbacks get called during maintain.
 */
#include "tt-entities.h"

#include "tt-test.h"


static void create_callback(TTEntityId id, void *user_data) {
    int *acc = (int *) user_data;
    *acc += id;
}


int main(void) {
    int expected = 0;
    int acc = 0;

    tt_entities_startup();

    tt_entities_create();
    tt_entities_create();
    tt_entities_create();
    tt_assert(acc == 0);

    tt_entities_maintain();
    tt_assert(acc == 0);

    int handle = tt_entities_bind_on_create_callback(create_callback, &acc);

    expected += tt_entities_create();
    expected += tt_entities_create();
    tt_assert(acc == 0);

    tt_entities_maintain();
    tt_assert(acc == expected);

    tt_entities_bind_on_create_callback(create_callback, &acc);

    expected += 2 * tt_entities_create();
    expected += 2 * tt_entities_create();

    tt_entities_maintain();
    tt_assert(acc == expected);

    tt_entities_unbind_on_create_callback(handle);

    expected += tt_entities_create();
    expected += tt_entities_create();

    tt_entities_maintain();
    tt_assert(acc == expected);

    tt_entities_shutdown();

    return 0;
}
