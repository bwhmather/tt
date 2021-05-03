/**
 * Checks that `tt::entities::release_id` will make an id available for reuse.
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
    tt::entities::startup();

    tt::StorageSparseVector<Component> *storage =
        new tt::StorageSparseVector<Component>();

    tt::entities::create();
    tt::entities::create();

    storage->add(tt::entities::create(), Component(4));

    tt::entities::shutdown();

    return 0;
}

