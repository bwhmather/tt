#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef size_t TTEntityId;
typedef size_t TTEntityIter;

void
tt_entities_startup(void);
void
tt_entities_shutdown(void);

void
tt_entities_maintain(void);

TTEntityId
tt_entities_create(void);
void
tt_entities_remove(TTEntityId entity_id);

int
tt_entities_bind_on_create_callback(
    void (*callback)(TTEntityId id, void *user_data), void *user_data
);
void
tt_entities_unbind_on_create_callback(int handle);

int
tt_entities_bind_on_delete_callback(
    void (*callback)(TTEntityId id, void *user_data), void *user_data
);
void
tt_entities_unbind_on_delete_callback(int handle);

void
tt_entities_iter_begin(TTEntityIter *iter);
bool
tt_entities_iter_has_next(TTEntityIter *iter);
TTEntityId
tt_entities_iter_next(TTEntityIter *iter);
