/**
 * Checks that copying an empty bitset doesn't crash.
 */
#include "tt-bitset.h"

#include <assert.h>
#include <stdint.h>

#include "tt-entities.h"

#include "tt-test.h"


int main(void) {
    /* Bitsets are a dependency of the entities module, and not the other way
     * round.  We don't need to call `tt_entities_startup` */
    TTBitset src_bitset;
    TTBitset tgt_bitset;

    tt_bitset_init(&src_bitset);
    tt_bitset_init(&tgt_bitset);

    tt_bitset_copy(&src_bitset, &tgt_bitset);
    for (TTEntityId id = 1; id < 2048; id++) {
        tt_assert(!tt_bitset_get(&tgt_bitset, id));
    }

    tt_bitset_destroy(&src_bitset);
    tt_bitset_destroy(&tgt_bitset);

    return 0;
}

