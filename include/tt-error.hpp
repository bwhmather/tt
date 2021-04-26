#pragma once

enum class TTLogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

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
        TTLogLevel::DEBUG, __VA_ARGS__                                      \
    );

#define tt_info(...) do {                                                   \
    tt_log_impl(                                                            \
        __FILE__, __LINE__, __func__,                                       \
        TTLogLevel::INFO, __VA_ARGS__                                       \
    );

#define tt_warning(...) do {                                                \
    tt_log_impl(                                                            \
        __FILE__, __LINE__, __func__,                                       \
        TTLogLevel::WARNING, __VA_ARGS__                                    \
    );

#define tt_error(...) do {                                                  \
    tt_log_impl(                                                            \
        __FILE__, __LINE__, __func__,                                       \
        TTLogLevel::ERROR, __VA_ARGS__                                      \
    );

[[ noreturn ]] void tt_abort_impl(
    const char *file, int line, const char *func,
    const char *format, ...
);

#define tt_abort(...)                                                       \
    tt_abort_impl(                                                          \
        __FILE__, __LINE__, __func__, __VA_ARGS__                           \
    );

void tt_abort_if_errno_impl(
    const char *file, int line, const char *func,
    const char *format, ...
);

#define tt_abort_if_errno(...)                                              \
    tt_abort_if_errno_impl(                                                 \
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
