extern "C" {
#include "tt-component-behaviour.h"
}

#include "tt-storage-sparse-vector.tpp"

extern "C" {
#include "bt.h"
#include "tt-error.h"
}

namespace state {
    static bool initialised = false;
    static TTStorageSparseVector<BTBehaviour *> *storage = NULL;
}

extern "C" void tt_component_behaviour_startup(void) {
    tt_assert(state::initialised == false);

    state::storage = new TTStorageSparseVector<BTBehaviour *>();

    state::initialised = true;
}

extern "C" void tt_component_behaviour_shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;

    state::initialised = false;
}


extern "C" void tt_component_behaviour_set(
    TTEntityId entity_id, BTBehaviour *behaviour
) {
    tt_assert(state::initialised == true);

    if (behaviour != NULL) {
        state::storage->add(entity_id, behaviour);
    } else {
        state::storage->remove(entity_id);
    }
}

extern "C" bool tt_component_behaviour_has(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->has(entity_id);
}

extern "C" BTBehaviour *tt_component_behaviour_get(
    TTEntityId entity_id
) {
    tt_assert(state::initialised == true);

    if (!state::storage->has(entity_id)) {
        return NULL;
    }

    return state::storage->get(entity_id);
}


extern "C" void tt_component_behaviour_remove(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->remove(entity_id);
}
