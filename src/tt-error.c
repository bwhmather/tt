#include "tt-error.h"

#include <stdarg.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

static const char *tt_error_level_str(TTLogLevel level) {
    switch (level) {
      case TT_LOG_LEVEL_INFO:
        return "INFO";
      case TT_LOG_LEVEL_DEBUG:
        return "DEBUG";
      case TT_LOG_LEVEL_WARNING:
        return "WARNING";
      case TT_LOG_LEVEL_ERROR:
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
    tt_log_impl_v(file, line, func, TT_LOG_LEVEL_ERROR, format, args);
    va_end(args);

    abort();
}

void tt_abort_errno_impl(
    const char *file, int line, const char *func,
    const char *format, ...
) {
    int errno_saved = errno;
    fprintf(
        stderr, "%s:%i %s: %s: ",
        file, line, func, tt_error_level_str(TT_LOG_LEVEL_ERROR)
    );

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    fprintf(stderr, ": %s\n", strerror(errno_saved));

    abort();
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
            file, line, func, tt_error_level_str(TT_LOG_LEVEL_ERROR)
        );

        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);

        fprintf(stderr, ": %s\n", tt_gl_error_str(error));

        abort();
    }
}
