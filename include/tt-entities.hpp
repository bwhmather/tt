#pragma once

#include "tt-types.hpp"

#include <stdbool.h>
#include <stdint.h>

namespace tt {

typedef size_t EntityIter;

namespace entities {

void startup(void);
void shutdown(void);

void maintain(void);

tt::EntityId create(void);
void remove(tt::EntityId entity_id);

int bind_on_create_callback(
    void (*callback) (tt::EntityId id, void *user_data), void *user_data
);
void unbind_on_create_callback(int handle);

int bind_on_delete_callback(
    void (*callback) (tt::EntityId id, void *user_data), void *user_data
);
void unbind_on_delete_callback(int handle);

void iter_begin(tt::EntityIter *iter);
bool iter_has_next(tt::EntityIter *iter);
tt::EntityId iter_next(tt::EntityIter *iter);

}  /* namespace entities */
}  /* namespace tt */
