#include "tt-texture.hpp"

#include <csetjmp>
#include <cstdio>
#include <cstring>
#include <memory>
#include <string>

#include <GL/glew.h>
#include <png.h>

#include "tt-error.hpp"


GLuint tt_load_texture(const std::string& filename) {
    FILE *fp = NULL;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    png_uint_32 width;
    png_uint_32 height;
    int bit_depth;
    int color_type;
    int interlace_type;

    unsigned long row_bytes;
    unsigned char* data = NULL;

    tt_debug("loading %s", filename);

    png_bytepp row_pointers = NULL;

    fp = fopen(filename.c_str(), "rb");
    tt_abort_if_errno("could not open image");

    png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, NULL, NULL, NULL
    );
    if (png_ptr == NULL) {
        tt_abort("could not allocate PNG read struct");
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        tt_abort("could not allocate PNG info struct");
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        tt_abort("libpng error");
    }

    png_init_io(png_ptr, fp);

    png_set_sig_bytes(png_ptr, 0);
    png_read_png(
        png_ptr, info_ptr,
        PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND,
        NULL
    );

    png_get_IHDR(
        png_ptr, info_ptr,
        &width, &height,
        &bit_depth, &color_type, &interlace_type,
        NULL, NULL
    );

    row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    data = (unsigned char*) malloc(row_bytes * height);

    row_pointers = png_get_rows(png_ptr, info_ptr);

    // Align bytes as OpenGL expects them.
    for (unsigned int i = 0; i < height; i++) {
        std::memcpy(data + (row_bytes * i), row_pointers[i], row_bytes);
    }

    png_destroy_info_struct(png_ptr, &info_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    fclose(fp);

    GLint internal_format = 0;

    switch (color_type) {
      case PNG_COLOR_TYPE_RGBA:
        internal_format = GL_RGBA;
        break;

      case PNG_COLOR_TYPE_RGB:
        internal_format = GL_RGB;
        break;

      case PNG_COLOR_TYPE_GRAY:
        internal_format = GL_LUMINANCE;
        break;

      case PNG_COLOR_TYPE_GRAY_ALPHA:
        internal_format = GL_LUMINANCE_ALPHA;
        break;

      default:
        tt_abort("unsupported PNG color type: %d", color_type);
    }

    // Find a free texture index or "name".
    GLuint texture;
    glGenTextures(1, &texture);
    tt_abort_if_gl_error("failed to generate texture objects");

    // Find the index to global state
    glBindTexture(GL_TEXTURE_2D, texture);
    tt_abort_if_gl_error("failed to bind texture object");

    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    tt_abort_if_gl_error("failed to set texture parameters");

    tt_debug("uploading %lu bytes to the gpu", row_bytes * height);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,  // Mip-map level.
        internal_format,  // Internal format.
        width, height,
        0,  // Border.
        internal_format,  // Data format.
        GL_UNSIGNED_BYTE,  // Data type.
        data
    );
    tt_abort_if_gl_error("failed to upload image data")

    glGenerateMipmap(GL_TEXTURE_2D);
    tt_abort_if_gl_error("failed to generate mipmaps")

    free(data);

    return texture;
}

