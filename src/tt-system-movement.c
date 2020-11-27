typedef struct {
    float position[2];
    float velocity[2];
    float max_speed;

    TTJob job;
    TTEntityId target_id;

    TTSpriteId sprite_id;

    bool has_target: 1;
    bool has_sprite: 1;
    bool has_position: 1;
    bool has_velocity: 1;
} TTEntity;


void tt_cursor_init(TTCursor *cursor) {
    *cursor = 0;
}

bool tt_cursor_has_next(TTCursor *cursor) {
    if (cursor
}

TTEntityId tt_cursor_next(TTCursor *cursor) {
    TTEntityId entity_id = (TTEntityId) *cursor;

    do {
        *cursor++;
    } while (*cursor <= tt_entities_max)
}




void tt_system_movement() {
    TTEntityIter *iter = tt_entity_iter_new(
        &cursor, 3,
        tt_entities_all(),
        tt_component_positions_all(),
        tt_component_velocities_all(),
    );

    for (
        tt_entity_iter_begin(iter);
        !tt_entity_iter_is_finished(iter);
        tt_entity_iter_next(iter)
    ) {
        TTEntityId entity_id = tt_entity_iter_get(&cursor);

    }

    tt_entity_iter_free(iter);
}


void tt_system_movement() {
    TTEntitySet *entities = tt_entity_set_new();

    tt_component_positions_filter(entities);
    tt_component_velocities_filter(entities);



    for (
        TTEntitySetIter iter = tt_entity_iter_new(entities);
        !tt_iter_is_finished(entities, iter);


    )

            int entity_id = 0; entity_id < tt_max_entity_id(); entity_id++) {
        if (!tt_entity.is_alive) continue;

    }
}

void tt_system_movement() {
    TTCursor cursor;

    tt_cursor_init(&cursor);

    while (tt_cursor_has_next(&cursor)) {
        TTEntityId entity_id = tt_cursor_next(&cursor);

        TTPosition *position = tt_get_position(entity_id);
        TTVelocity *velocity = tt_get_velocity(entity_id);

        if (position == NULL) continue;
        if (velocity == NULL) continue;

        position->x += dt * velocity->x;
        position->y += dt * velocity->y;
    }
}





void tt_system_movement() {
    for (int entity_id = 0; entity_id < num_entities; entity_id ++) {
        entity = &entities[entity_id]

        if (!entity.is_alive) continue;
        if (!entity.has_position) continue;
        if (!entity.has_velocity) continue;

        entity->position[0] += entity->velocity[0] * dt;
        entity->position[1] += entity->velocity[1] * dt;
    }
}



void tt_system_untarget_dead() {
    for (int entity_id = 0; entity_id < num_entities; entity_id ++) {
        TTEntity *entity = &entities[entity_id];

        if (!entity.is_alive) continue;
        if (entity.target_id == 0) continue;

        TTEntity *target = &entities[entity.target_id];
        if (!target.is_alive) {
            entity.target_id = 0;
            entity.job = TT_JOB_IDLE;
        }
    }
}

void tt_system_rebuild_free_list() {
    for (int entity_id = 0; entity_id < num_entities; entity_id++) {
        TTEntityId *entity = &entities[entity_id];

        if (entity.is_alive) continue;

    }
}



void tt_system_movement() {
    for (TTEntity : tt_entities()) {
        TTEntityId entity_id = tt_cursor_next(&cursor);

        TTPosition *position = tt_get_position(entity_id);
        TTVelocity *velocity = tt_get_velocity(entity_id);

        if (position == NULL) continue;
        if (velocity == NULL) continue;

        position->x += dt * velocity->x;
        position->y += dt * velocity->y;

    }
}












