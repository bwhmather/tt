/**
 * Checks that bitsets basically work.
 */
#include "tt-bitset.h"

#include <assert.h>
#include <stdint.h>

#include "tt-entities.h"

#include "tt-test.h"


int main(void) {
    /* Bitsets are a dependency of the entities module, and not the other way
     * round.  We don't need to call `tt_entities_startup` */
    TTBitset bitset;

    tt_bitset_init(&bitset);

    tt_assert(!tt_bitset_get(&bitset, 10000));

    tt_bitset_set(&bitset, 10000);
    tt_assert(tt_bitset_get(&bitset, 10000));

    tt_bitset_unset(&bitset, 10000);
    tt_assert(!tt_bitset_get(&bitset, 10000));

    tt_bitset_clear(&bitset);

    return 0;
}

