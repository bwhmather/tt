#pragma once

#include <assert.h>
#include <type_traits>
#include <vector>


template<typename T>
struct Wrapper {
private:
    T m_value;

public:
    Wrapper() {
        static_assert(std::is_standard_layout<Wrapper<T>>::value);
        static_assert(sizeof(T) == sizeof(Wrapper<T>));
    }

    ~Wrapper() : m_value{} {}

    void set(T value) {
        m_value = value;
    }

    T& get() {
        return m_value;
    }

    void unset() {
        m_value.~T();
    }
};

template<typename T>
class TTStorageVector final {
    std::vector<bool> m_mask;
    std::vector<Wrapper<T>> m_data;

  public:
    TTStorageVector() : m_mask(), m_data() {
        // TODO bind entity deleted callback.
    }

    ~TTStorageVector() {
        for (TTEntityId entity_id = 0;  entity_id < m_mask.size(); entity_id++) {
            if (m_mask[entity_id]) {
                m_data[entity_id].unset();
            }
        }
        // TODO unbind entity deleted callback.
    }

    TTStorageVector(const TTStorageVector &) = delete;
    TTStorageVector(TTStorageVector &&) = delete;

    void add(TTEntityId entity_id, T value) {
        if (entity_id >= m_mask.size()) {
            m_mask.resize(entity_id + 1, false);
            m_data.resize(entity_id + 1);
        }

        if (m_mask[entity_id]) {
            m_data[entity_id].unset();
        }

        m_mask[entity_id] = true;
        m_data[entity_id].set(value);
    }

    bool has(TTEntityId entity_id) {
        if (entity_id >= m_mask.size()) {
            return false;
        }

        return m_mask.at(entity_id);
    }

    T& get(TTEntityId entity_id) {
        assert(has(entity_id));
        return m_data[entity_id].get();
    }

    void remove(TTEntityId entity_id) {
        assert(has(entity_id));
        m_mask[entity_id] = false;
        m_data[entity_id].unset();
    }
};
