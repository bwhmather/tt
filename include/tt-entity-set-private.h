#pragma once

#include "tt-entity-set.h"

#include <stdint.h>

struct TTEntitySet {
    size_t nblocks;
    uint64_t *mask;
};

