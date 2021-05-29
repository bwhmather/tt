#include "tt-renderer.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <cglm/cglm.h>

#include "tt-error.h"
#include "tt-resource-camera.h"
#include "tt-texture.h"


static const char* VERTEX_SHADER_TEXT =
    "#version 110\n"
    "uniform mat4 camera_matrix;\n"
    "\n"
    "attribute vec3 vertex_position;\n"
    "attribute vec2 vertex_tex_coord;\n"
    "\n"
    "varying vec2 fragment_tex_coord;\n"
    "\n"
    "void main() {\n"
    "    gl_Position = camera_matrix * vec4(vertex_position, 1.0);\n"
    "    fragment_tex_coord = vertex_tex_coord;\n"
    "}\n";

static const char* FRAGMENT_SHADER_TEXT =
    "#version 110\n"
    "uniform sampler2D spritesheet;\n"
    "\n"
    "varying vec2 fragment_tex_coord;\n"
    "\n"
    "void main() {\n"
    "    vec4 colour = texture2D(spritesheet, fragment_tex_coord);\n"
    "    if (colour.a == 0.0) {\n"
    "        discard;\n"
    "    } else {\n"
    "        gl_FragColor = colour;\n"
    "    }\n"
    "}\n";

static struct TTRendererState {
    bool initialised;

    GLuint vertex_array;

    size_t buffer_size;
    size_t buffer_capacity;
    TTVertex *buffer_data;
    GLuint buffer;

    GLuint shader_program;
    GLuint spritesheet;

    GLint camera_matrix_location;
    GLint spritesheet_location;
    GLint position_location;
    GLint tex_coord_location;
} state = { .initialised = false };

void tt_renderer_startup(void) {
    tt_assert(state.initialised == false);

    GLuint vertex_shader, fragment_shader;

    tt_assert(state.buffer_data == NULL);

    state.buffer_capacity = 256;
    state.buffer_size = 0;
    state.buffer_data = (TTVertex *) malloc(
        sizeof(TTVertex) * state.buffer_capacity
    );
    tt_assert(state.buffer_data != NULL);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &VERTEX_SHADER_TEXT, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &FRAGMENT_SHADER_TEXT, NULL);
    glCompileShader(fragment_shader);

    state.shader_program = glCreateProgram();
    glAttachShader(state.shader_program, vertex_shader);
    glAttachShader(state.shader_program, fragment_shader);
    glLinkProgram(state.shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    state.camera_matrix_location = glGetUniformLocation(
        state.shader_program, "camera_matrix"
    );
    state.spritesheet_location = glGetAttribLocation(
        state.shader_program, "spritesheet"
    );
    state.position_location = glGetAttribLocation(
        state.shader_program, "vertex_position"
    );
    state.tex_coord_location = glGetAttribLocation(
        state.shader_program, "vertex_tex_coord"
    );

    /* Configure vertex array stride and size */
    glGenVertexArrays(1, &state.vertex_array);
    glGenBuffers(1, &state.buffer);

    glBindVertexArray(state.vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, state.buffer);

    glEnableVertexAttribArray(state.position_location);
    glVertexAttribPointer(
        state.position_location, 3, GL_FLOAT, GL_FALSE,
        sizeof(TTVertex), (void*) 0
    );
    glEnableVertexAttribArray(state.tex_coord_location);
    glVertexAttribPointer(
        state.tex_coord_location, 2, GL_FLOAT, GL_FALSE,
        sizeof(TTVertex), (void*) (sizeof(float) * 3)
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    state.spritesheet = tt_load_texture("spritesheet.png");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    state.initialised = true;
}

void tt_renderer_shutdown(void) {
    tt_assert(state.initialised == true);

    free(state.buffer_data);

    glDeleteVertexArrays(1, &state.vertex_array);
    glDeleteBuffers(1, &state.buffer);
    glDeleteProgram(state.shader_program);
    glDeleteTextures(1, &state.spritesheet);

    state.initialised = false;
}

void tt_renderer_push_vertex(TTVertex *v) {
    tt_assert(state.initialised == true);

    if (state.buffer_size >= state.buffer_capacity) {
        state.buffer_capacity += state.buffer_capacity / 2;

        state.buffer_data = (TTVertex *) realloc(
            state.buffer_data,
            sizeof(TTVertex) * state.buffer_capacity
        );
        tt_assert(state.buffer_data == NULL);
    }

    memcpy(
        &state.buffer_data[state.buffer_size],
        v, sizeof(TTVertex)
    );

    state.buffer_size++;
}


void tt_renderer_do_render(void) {
    tt_assert(state.initialised == true);

    mat4 camera_matrix;
    tt_resource_camera_get_matrix(camera_matrix);

    glUseProgram(state.shader_program);

    glUniformMatrix4fv(
        state.camera_matrix_location,
        1, GL_FALSE, (float *) camera_matrix
    );

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, state.spritesheet);
    glUniform1i(state.spritesheet_location, 0);

    glBindVertexArray(state.vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, state.buffer);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(TTVertex) * state.buffer_size, state.buffer_data,
        GL_STREAM_DRAW
    );

    glDrawArrays(GL_TRIANGLES, 0, state.buffer_size);

    glBindTexture(GL_TEXTURE_2D, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Empty the render buffer, but don't free the memory..
    state.buffer_size = 0;
}
