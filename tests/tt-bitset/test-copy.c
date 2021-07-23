/**
 * Checks that a copied bitset contains what was written to the source.
 */
#include "tt-bitset.h"

#include <assert.h>
#include <stdint.h>

#include "tt-entities.h"

#include "tt-test.h"


int main(void) {
    uint32_t rnd_state;

    /* Bitsets are a dependency of the entities module, and not the other way
     * round.  We don't need to call `tt_entities_startup` */
    TTBitset src_bitset;
    TTBitset tgt_bitset;

    tt_bitset_init(&src_bitset);
    tt_bitset_init(&tgt_bitset);

    rnd_state = 0xdeadbeef;
    for (TTEntityId id = 1; id < 2048; id++) {
        if (tt_rnd(&rnd_state) & 1) {
            tt_bitset_set(&src_bitset, id);
        }
    }

    tt_bitset_copy(&src_bitset, &tgt_bitset);

    rnd_state = 0xdeadbeef;
    for (TTEntityId id = 1; id < 2048; id++) {
        tt_assert(
            tt_bitset_get(&tgt_bitset, id) == (tt_rnd(&rnd_state) & 1 ? 1 : 0)
        );
    }

    tt_bitset_clear(&src_bitset);
    tt_bitset_clear(&tgt_bitset);

    return 0;
}

