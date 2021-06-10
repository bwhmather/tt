#pragma once

#include <stddef.h>


typedef enum {
    BT_RUNNING,
    BT_SUCCEEDED,
    BT_FAILED
} BTResult;

typedef struct BTBehaviour BTBehaviour;
typedef struct BTContext BTContext;

typedef void (*BTInitFn)(BTBehaviour *, void *, void *);
typedef BTResult (*BTTickFn)(BTBehaviour *, void *, void *);
typedef void (*BTInterruptFn)(BTBehaviour *, void *, void *);
typedef void (*BTFreeFn)(BTBehaviour *);

struct BTBehaviour {
    BTInitFn init;
    BTTickFn tick;
    BTInterruptFn interrupt;

    size_t frame_size;

    BTFreeFn free;
};

void bt_context_init(BTContext *context, size_t size);
BTResult bt_run(BTBehaviour *behaviour, BTContext *context, void *user_data);
BTResult bt_delegate(BTBehaviour *behaviour);
void bt_behaviour_free(BTBehaviour *behaviour);
