#pragma once

#include <csetjmp>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>


#define tt_assert(tt_test_condition) do {                                   \
        if (!(tt_test_condition)) {                                         \
            fprintf(                                                        \
                stderr, "%s:%i %s: Assertion `%s' failed",                  \
                __FILE__, __LINE__, __func__, #tt_test_condition            \
            );                                                              \
            abort();                                                        \
        }                                                                   \
    } while (0)

static jmp_buf tt_test_on_sigabrt_jump_target;
static inline void tt_test_on_sigabrt(int signal) {
    (void) signal;  /* unused */
    longjmp(tt_test_on_sigabrt_jump_target, 1);
}

#define tt_assert_aborts(tt_test_expression) do {                           \
        struct sigaction tt_test_handler_old;                               \
        struct sigaction tt_test_handler_new;                               \
        memset(&tt_test_handler_new, 0, sizeof(tt_test_handler_new));       \
        sigemptyset(&tt_test_handler_new.sa_mask);                          \
        tt_test_handler_new.sa_handler = &tt_test_on_sigabrt;               \
        sigaction(SIGABRT, &tt_test_handler_new, &tt_test_handler_old);     \
        int tt_test_abort_called = setjmp(tt_test_on_sigabrt_jump_target);  \
        if (!tt_test_abort_called) {                                        \
            tt_test_expression;                                             \
        }                                                                   \
        sigaction(SIGABRT, &tt_test_handler_old, NULL);                     \
        if (!tt_test_abort_called) {                                        \
            fprintf(                                                        \
                stderr, "%s:%i %s: Expression `%s' did not abort",          \
                __FILE__, __LINE__, __func__, #tt_test_expression           \
            );                                                              \
            abort();                                                        \
        }                                                                   \
    } while(0)


/**
 * LCG random number generator.
 * See http://en.wikipedia.org/wiki/Linear_congruential_generator.
 */
static inline uint8_t tt_rnd(uint32_t *state) {
    const uint32_t A = 1664525;
    const uint32_t C = 1013904223;

    *state = A * (*state) + C;
    return (uint8_t) (((*state) >> 24) & 0xff);
}

