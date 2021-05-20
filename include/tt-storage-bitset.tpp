#pragma once

#include <cassert>
#include <memory>
#include <type_traits>
#include <vector>


static void on_delete_callback(TTEntityId entity_id, void *data);

class TTStorageBitset final {
    std::vector<bool> m_mask;

    int delete_callback_handle;

  public:
    TTStorageBitset() : m_mask() {
        delete_callback_handle = tt_entities_bind_on_delete_callback(
            &on_delete_callback, this
        );
    }

    ~TTStorageBitset() {
        tt_entities_unbind_on_delete_callback(delete_callback_handle);
    }

    TTStorageBitset(const TTStorageBitset &) = delete;
    TTStorageBitset(TTStorageBitset &&) = delete;

    void set(TTEntityId entity_id, bool value) {
        if (entity_id >= m_mask.size()) {
            m_mask.resize(entity_id + 1, false);
        }
        m_mask[entity_id] = value;
    }

    bool get(TTEntityId entity_id) {
        if (entity_id >= m_mask.size()) {
            return false;
        }

        return m_mask[entity_id];
    }
};

static void on_delete_callback(TTEntityId entity_id, void *data) {
    TTStorageBitset& storage = *((TTStorageBitset *) data);

    if (storage.get(entity_id)) {
        storage.set(entity_id, false);
    }
}
