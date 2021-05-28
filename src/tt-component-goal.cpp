#include "tt-component-goal.hpp"

extern "C" {
#include "tt-error.h"
}
#include "tt-storage-sparse-vector.tpp"

namespace state {
    static bool initialised = false;
    static TTStorageSparseVector<TTGoal> *storage = NULL;
}

void tt_component_goal_startup(void) {
    tt_assert(state::initialised == false);

    state::storage = new TTStorageSparseVector<TTGoal>();

    state::initialised = true;
}

void tt_component_goal_shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;

    state::initialised = false;
}

void tt_component_goal_set(TTEntityId entity_id, TTGoal goal) {
    tt_assert(state::initialised == true);

    return state::storage->add(entity_id, goal);
}

bool tt_component_goal_has(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->has(entity_id);
}

TTGoal tt_component_goal_get(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->get(entity_id);
}

void tt_component_goal_remove(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->remove(entity_id);
}
