#include "tt-error.hpp"

#include <cstdarg>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <GL/glew.h>

static const char *tt_error_level_str(TTLogLevel level) {
    switch (level) {
      case TTLogLevel::INFO:
        return "INFO";
      case TTLogLevel::DEBUG:
        return "DEBUG";
      case TTLogLevel::WARNING:
        return "WARNING";
      case TTLogLevel::ERROR:
        return "ERROR";
      default:
        return "LOG";
    }
}

static const char *tt_gl_error_str(GLenum error) {
    switch (error) {
      case GL_NO_ERROR:
        return "no error";
      case GL_INVALID_ENUM:
        return "invalid enum";
      case GL_INVALID_VALUE:
        return "invalid value";
      case GL_INVALID_OPERATION:
        return "invalid operation";
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "invalid framebuffer operation";
      case GL_OUT_OF_MEMORY:
        return "out of memory";
      case GL_STACK_UNDERFLOW:
        return "stack underflow";
      case GL_STACK_OVERFLOW:
        return "stack overflow";
      default:
        return "unknown error";
    }
}

static void tt_log_impl_v(
    const char *file, int line, const char *func,
    TTLogLevel level, const char *format, va_list args
) {
    fprintf(
        stderr, "%s:%i %s: %s: ",
        file, line, func, tt_error_level_str(level)
    );
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
}

void tt_log_impl(
    const char *file, int line, const char *func,
    TTLogLevel level, const char *format, ...
) {
    va_list args;
    va_start(args, format);
    tt_log_impl_v(file, line, func, level, format, args);
    va_end(args);
}

[[ noreturn ]] void tt_abort_impl(
    const char *file, int line, const char *func,
    const char *format, ...
) {
    va_list args;
    va_start(args, format);
    tt_log_impl_v(file, line, func, TTLogLevel::ERROR, format, args);
    va_end(args);

    abort();
}

void tt_abort_if_errno_impl(
    const char *file, int line, const char *func,
    const char *format, ...
) {
    if (errno) {
        fprintf(
            stderr, "%s:%i %s: %s: ",
            file, line, func, tt_error_level_str(TTLogLevel::ERROR)
        );

        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);

        fprintf(stderr, ": %s\n", strerror(errno));

        abort();
    }
}

void tt_abort_if_gl_error_impl(
    const char *file, int line, const char *func,
    const char *format, ...
) {
    GLenum error;

    error = glGetError();

    if (error != GL_NO_ERROR) {
        fprintf(
            stderr, "%s:%i %s: %s: ",
            file, line, func, tt_error_level_str(TTLogLevel::ERROR)
        );

        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);

        fprintf(stderr, ": %s\n", tt_gl_error_str(error));

        abort();
    }
}
