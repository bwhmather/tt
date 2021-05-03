#pragma once

#include <cassert>
#include <memory>
#include <type_traits>
#include <vector>

namespace tt {

template<typename T>
static void on_delete_callback(tt::EntityId entity_id, void *data);

template<typename T>
class StorageSparseVector final {
    std::vector<bool> m_mask;

    T* m_data;
    size_t m_data_capacity;

    int delete_callback_handle;

  public:
    StorageSparseVector() : m_mask(), m_data(NULL), m_data_capacity(0) {
        delete_callback_handle = tt::entities::bind_on_delete_callback(
            &on_delete_callback<T>, this
        );
    }

    ~StorageSparseVector() {
        assert(m_data_capacity >= m_mask.size());

        for (
            tt::EntityId entity_id = 0;
            entity_id < m_mask.size();
            entity_id++
        ) {
            if (m_mask[entity_id]) {
                m_data[entity_id].~T();
            }
        }

        std::allocator<T>().deallocate(m_data, m_data_capacity);
        m_data_capacity = 0;

        tt::entities::unbind_on_delete_callback(delete_callback_handle);
    }

    StorageSparseVector(const StorageSparseVector &) = delete;
    StorageSparseVector(StorageSparseVector &&) = delete;

    void add(tt::EntityId entity_id, T value) {
        if (entity_id >= m_mask.size()) {
            m_mask.resize(entity_id + 1, false);
        }

        if (entity_id >= m_data_capacity) {
            size_t new_capacity = m_data_capacity;

            if (new_capacity == 0) {
                new_capacity = 16;
            }

            while (entity_id >= new_capacity) {
                new_capacity += new_capacity / 2;
            }

            T* new_data = std::allocator<T>().allocate(new_capacity);

            for (
                tt::EntityId move_id = 0;
                move_id < m_mask.size();
                move_id++
            ) {
                if (m_mask[move_id]) {
                    new_data[move_id] = std::move_if_noexcept(m_data[move_id]);
                }
            }
            // TODO call destructors on new array values on exception.

            for (
                tt::EntityId free_id = 0;
                free_id < m_mask.size();
                free_id++
            ) {
                if (m_mask[free_id]) {
                    m_data[free_id].~T();
                }
            }

            std::allocator<T>().deallocate(m_data, m_data_capacity);
            m_data = new_data;
            m_data_capacity = new_capacity;
        }


        if (m_mask[entity_id]) {
            m_data[entity_id].~T();
        }

        m_data[entity_id] = std::move(value);
        // Order important.  `std::move` can throw.  We assume that the new
        // location is left in a meaningless state and don't try to clean up.
        m_mask[entity_id] = true;
    }

    bool has(tt::EntityId entity_id) {
        if (entity_id >= m_mask.size()) {
            return false;
        }

        return m_mask.at(entity_id);
    }

    T& get(tt::EntityId entity_id) {
        assert(has(entity_id));
        return m_data[entity_id];
    }

    void remove(tt::EntityId entity_id) {
        assert(has(entity_id));
        m_mask[entity_id] = false;
        m_data[entity_id].~T();
    }
};

template<typename T>
static void on_delete_callback(tt::EntityId entity_id, void *data) {
    StorageSparseVector<T>& storage = *((StorageSparseVector<T> *) data);

    if (storage.has(entity_id)) {
        storage.remove(entity_id);
    }
}

}  /* namespace tt */
