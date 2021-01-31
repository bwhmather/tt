#pragma once

#include <cassert>
#include <memory>
#include <type_traits>
#include <vector>


template<typename T>
static void on_delete_callback(TTEntityId entity_id, void *data);

template<typename T>
class TTStorageVector final {
    std::vector<bool> m_mask;

    T* m_data;
    size_t m_data_capacity;

    int delete_callback_handle;

  public:
    TTStorageVector() : m_mask(), m_data(NULL), m_data_capacity(0) {
        delete_callback_handle = tt_entities_bind_on_delete_callback(
            &on_delete_callback<T>, this
        );
    }

    ~TTStorageVector() {
        assert(m_data_capacity >= m_mask.size());

        for (TTEntityId entity_id = 0;  entity_id < m_mask.size(); entity_id++) {
            if (m_mask[entity_id]) {
                m_data[entity_id].~T();
            }
        }

        std::allocator<T>().deallocate(m_data, m_data_capacity);
        m_data_capacity = 0;

        tt_entities_unbind_on_delete_callback(delete_callback_handle);
    }

    TTStorageVector(const TTStorageVector &) = delete;
    TTStorageVector(TTStorageVector &&) = delete;

    void add(TTEntityId entity_id, T value) {
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

            for (TTEntityId move_id = 0; move_id < m_mask.size(); move_id++) {
                if (m_mask[move_id]) {
                    new_data[move_id] = std::move_if_noexcept(m_data[move_id]);
                }
            }
            // TODO call destructors on new array values on exception.

            for (TTEntityId free_id = 0; free_id < m_mask.size(); free_id++) {
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

    bool has(TTEntityId entity_id) {
        if (entity_id >= m_mask.size()) {
            return false;
        }

        return m_mask.at(entity_id);
    }

    T& get(TTEntityId entity_id) {
        assert(has(entity_id));
        return m_data[entity_id];
    }

    void remove(TTEntityId entity_id) {
        assert(has(entity_id));
        m_mask[entity_id] = false;
        m_data[entity_id].~T();
    }
};

template<typename T>
static void on_delete_callback(TTEntityId entity_id, void *data) {
    TTStorageVector<T>& storage = *((TTStorageVector<T> *) data);

    if (storage.has(entity_id)) {
        storage.remove(entity_id);
    }
}
