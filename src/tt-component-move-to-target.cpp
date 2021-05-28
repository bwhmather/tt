#include "tt-component-move-to-target.hpp"
extern "C" {
#include "tt-error.h"
}
#include "tt-storage-sparse-vector.tpp"

namespace state {
    static bool initialised = false;
    static TTStorageSparseVector<double> *storage = NULL;
}

void tt_component_move_to_target_startup(void) {
    tt_assert(state::initialised == false);

    state::storage = new TTStorageSparseVector<double>();
    tt_assert(state::storage != NULL);

    state::initialised = true;
}

void tt_component_move_to_target_shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;

    state::initialised = false;
}

void tt_component_move_to_target_set_target_range(
    TTEntityId entity_id, double range
) {
    tt_assert(state::initialised == true);

    state::storage->add(entity_id, range);
}

bool tt_component_move_to_target_has(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->has(entity_id);
}

double tt_component_move_to_target_get_target_range(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->get(entity_id);
}

void tt_component_move_to_target_remove(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    state::storage->remove(entity_id);
}
