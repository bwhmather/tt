/**
 * Checks that `tt_entities_release_id` will make an id available for reuse.
 */
#include "tt-entities.hpp"
#include "tt-storage-sparse-vector.tpp"

#include "tt-test.hpp"


class Component {
    int m_v;

public:
    Component() = delete;
    Component(int v) : m_v(v) {}
};

int main(void) {
    tt_entities_startup();

    TTStorageSparseVector<Component> *storage = new TTStorageSparseVector<Component>();

    tt_entities_create();
    tt_entities_create();

    storage->add(tt_entities_create(), Component(4));

    tt_entities_shutdown();

    return 0;
}

