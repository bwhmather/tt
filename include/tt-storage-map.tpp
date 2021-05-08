#pragma once

#include <cassert>
#include <memory>
#include <type_traits>
#include <vector>

template<typename T>
struct TTStorageMapEntry {
  private:
    TTEntityId m_entity_id;
    union {T m_value};

  public:
    TTStorageMapEntry() : m_entity_id(0) {}

    TTStorageMapEntry(const TTStorageMapEntry &) = delete;

    TTStorageMapEntry(TTStorageMapEntity &&src) {
        unset();
        m_entity_id = src.m_entity_id;
        if (m_entity_id) {
            src.m_value = 0;
            m_value = std::move(src.m_value);
        }
    }

    ~TTStorageMapEntry() {
        unset();
    }

    void set(TTEntityId entity_id, T value) {
        unset();
        m_entity_id = entity_id;
        m_value = value;
    }

    void unset() {
        if (m_entity_id) {
            m_value::~T();
        }
        m_entity_id = 0;
    }

    TTEntityId entity_id() {
        return m_entity_id;
    }

    T& value() {
        assert(m_entity_id);
        return m_value;
    }
}

template<typename T>
static void on_delete_callback(TTEntityId entity_id, void *data);

template<typename T>
class TTStorageMap final {
    std::vector<TTStorageMapEntry> m;

    int delete_callback_handle;

  public:
    TTStorageMap() : m_mask(), m_data(NULL), m_data_capacity(0) {
        delete_callback_handle = tt_entities_bind_on_delete_callback(
            &on_delete_callback<T>, this
        );
    }

    ~TTStorageMap() {
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

    TTStorageMap(const TTStorageMap &) = delete;
    TTStorageMap(TTStorageMap &&) = delete;

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

            for (
                TTEntityId move_id = 0;
                move_id < m_mask.size();
                move_id++
            ) {
                if (m_mask[move_id]) {
                    new_data[move_id] = std::move_if_noexcept(m_data[move_id]);
                }
            }
            // TODO call destructors on new array values on exception.

            for (
                TTEntityId free_id = 0;
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
    TTStorageMap<T>& storage = *((TTStorageMap<T> *) data);

    if (storage.has(entity_id)) {
        storage.remove(entity_id);
    }
}

}  /* namespace tt */
