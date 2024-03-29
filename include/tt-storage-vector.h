#pragma once

#include "tt-entities.h"

typedef struct TTStorageVector TTStorageVector;

TTStorageVector *
tt_storage_vector_new(size_t component_size);
void
tt_storage_vector_free(TTStorageVector *storage);

bool
tt_storage_vector_has(TTStorageVector *storage, TTEntityId entity_id);
void *
tt_storage_vector_add(TTStorageVector *storage, TTEntityId entity);
void *
tt_storage_vector_get(TTStorageVector *storage, TTEntityId entity);
void
tt_storage_vector_remove(TTStorageVector *storage, TTEntityId entity);
