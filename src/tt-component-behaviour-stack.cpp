extern "C" {
#include "tt-component-behaviour-stack.h"
}

#include <cstddef>

#include "tt-storage-sparse-vector.tpp"

extern "C" {
#include "bt.h"
#include "tt-behaviour.h"
#include "tt-error.h"
}


const std::size_t STACK_SIZE = 128;

struct TTPlaceholder {
    alignas(void *) char data[STACK_SIZE];
};


namespace state {
    static bool initialised = false;
    static TTStorageSparseVector<TTPlaceholder> *storage = NULL;
}

extern "C" void tt_component_behaviour_stack_startup(void) {
    tt_assert(state::initialised == false);

    state::storage = new TTStorageSparseVector<TTPlaceholder>();

    state::initialised = true;
}

extern "C" void tt_component_behaviour_stack_shutdown(void) {
    tt_assert(state::initialised == true);

    delete state::storage;
    state::storage = NULL;

    state::initialised = false;
}

extern "C" void tt_component_behaviour_stack_init(TTEntityId entity_id) {
    tt_assert(state::initialised == true);
    tt_assert(!state::storage->has(entity_id));

    state::storage->add(entity_id, { 0 });

    BTContext *stack = (BTContext *) state::storage->get(entity_id).data;

    bt_context_init(stack, STACK_SIZE);
}

extern "C" bool tt_component_behaviour_stack_has(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return state::storage->has(entity_id);
}

extern "C" BTContext *tt_component_behaviour_stack_get(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    return (BTContext *) state::storage->get(entity_id).data;
}

extern "C" void tt_component_behaviour_stack_remove(TTEntityId entity_id) {
    tt_assert(state::initialised == true);

    tt_assert(state::storage->has(entity_id));

    // TODO check that we aren't destroying any running behaviours.

    return state::storage->remove(entity_id);
}
