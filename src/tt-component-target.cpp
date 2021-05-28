#include "tt-component-target.hpp"

#include <assert.h>
extern "C" {
#include "tt-error.h"
}
#include "tt-storage-sparse-vector.tpp"

namespace state {
    static bool initialised = false;
    static TTStorageSparseVector<TTEntityId> *storage = NULL;
}

void tt_component_target_startup(void) {
    tt_assert(state::initialised == false);

    state::storage = new TTStorageSparseVector<TTEntityId>();

    state::initialised = true;
}

void tt_component_target_shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;

    state::initialised = false;
}

void tt_component_target_set(TTEntityId entity_id, TTEntityId target) {
    tt_assert(state::initialised == true);

    return state::storage->add(entity_id, target);
}

bool tt_component_target_has(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->has(entity_id);
}

TTEntityId tt_component_target_get(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->get(entity_id);
}

void tt_component_target_remove(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->remove(entity_id);
}
