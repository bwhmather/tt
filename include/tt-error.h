#pragma once

#ifdef __cplusplus
#define NO_RETURN [[ noreturn ]]
#else
#define NO_RETURN _Noreturn
#endif

typedef enum {
    TT_LOG_LEVEL_DEBUG,
    TT_LOG_LEVEL_INFO,
    TT_LOG_LEVEL_WARNING,
    TT_LOG_LEVEL_ERROR
} TTLogLevel;

void tt_log_impl(
    const char *file, int line, const char *func,
    TTLogLevel level, const char *format, ...
);

#define tt_log(level, ...)                                                  \
    tt_log_impl(                                                            \
        __FILE__, __LINE__, __func__, level, __VA_ARGS__                    \
    );

#define tt_debug(...)                                                       \
    tt_log_impl(                                                            \
        __FILE__, __LINE__, __func__,                                       \
        TT_LOG_LEVEL_DEBUG, __VA_ARGS__                                      \
    );

#define tt_info(...) do {                                                   \
    tt_log_impl(                                                            \
        __FILE__, __LINE__, __func__,                                       \
        TT_LOG_LEVEL_INFO, __VA_ARGS__                                       \
    );

#define tt_warning(...) do {                                                \
    tt_log_impl(                                                            \
        __FILE__, __LINE__, __func__,                                       \
        TT_LOG_LEVEL_WARNING, __VA_ARGS__                                    \
    );

#define tt_error(...) do {                                                  \
    tt_log_impl(                                                            \
        __FILE__, __LINE__, __func__,                                       \
        TT_LOG_LEVEL_ERROR, __VA_ARGS__                                      \
    );

NO_RETURN void tt_abort_impl(
    const char *file, int line, const char *func,
    const char *format, ...
);

#define tt_abort(...)                                                       \
    tt_abort_impl(                                                          \
        __FILE__, __LINE__, __func__, __VA_ARGS__                           \
    );

NO_RETURN void tt_abort_errno_impl(
    const char *file, int line, const char *func,
    const char *format, ...
);

#define tt_abort_errno(...)                                                 \
    tt_abort_errno_impl(                                                    \
        __FILE__, __LINE__, __func__, __VA_ARGS__                           \
    );

void tt_abort_if_gl_error_impl(
    const char *file, int line, const char *func,
    const char *format, ...
);

#define tt_abort_if_gl_error(...)                                           \
    tt_abort_if_gl_error_impl(                                              \
        __FILE__, __LINE__, __func__, __VA_ARGS__                           \
    );

#define tt_assert(tt_test_condition) do {                                   \
        if (!(tt_test_condition)) {                                         \
            tt_abort("assertion failed: %s", #tt_test_condition)            \
        }                                                                   \
    } while (0)
