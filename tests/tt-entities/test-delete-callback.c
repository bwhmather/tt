/**
 * Checks that delete callbacks get called during maintain.
 */
#include "tt-entities.h"

#include "tt-test.h"


static void delete_callback(TTEntityId id, void *user_data) {
    int *acc = (int *) user_data;
    *acc += id;
}


int main(void) {
    int expected = 0;
    int acc = 0;

    tt_entities_startup();
    for (int i = 0; i < 10; i++) {
        tt_entities_create();
    }
    tt_entities_maintain();

    tt_entities_remove(1);
    tt_entities_remove(2);
    tt_entities_remove(3);
    tt_assert(acc == 0);

    tt_entities_maintain();
    tt_assert(acc == 0);

    int handle = tt_entities_bind_on_delete_callback(delete_callback, &acc);

    tt_entities_remove(4);
    tt_entities_remove(5);
    tt_assert(acc == 0);

    tt_entities_maintain();
    tt_assert(acc == 4 + 5);

    tt_entities_bind_on_delete_callback(delete_callback, &acc);

    tt_entities_remove(6);
    tt_entities_remove(7);
    tt_assert(acc == 4 + 5);

    tt_entities_maintain();
    tt_assert(acc == 4 + 5 + 2 * 6 + 2 * 7);

    tt_entities_unbind_on_delete_callback(handle);

    tt_entities_remove(8);
    tt_entities_remove(9);
    tt_assert(acc == 4 + 5 + 2 * 6 + 2 * 7);

    tt_entities_maintain();
    tt_assert(acc == 4 + 5 + 2 * 6 + 2 * 7 + 8 + 9);

    tt_entities_shutdown();

    return 0;
}
