#include "tt-renderer.hpp"

#include <cassert>
#include <cstdlib>
#include <cstring>

#include <GL/glew.h>

#include "linmath.hpp"


static const char* VERTEX_SHADER_TEXT =
    "#version 110\n"
    "uniform mat4 camera_matrix;\n"
    "attribute vec3 vertex_position;\n"
    "attribute vec2 vertex_tex_coord;\n"
    "varying vec3 color;\n"
    "void main() {\n"
    "    gl_Position = camera_matrix * vec4(vertex_position, 1.0);\n"
    "    color = vec3(vertex_tex_coord, 1.0);\n"
    "}\n";

static const char* FRAGMENT_SHADER_TEXT =
    "#version 110\n"
    "varying vec3 color;\n"
    "void main() {\n"
    "    gl_FragColor = vec4(color, 1.0);\n"
    "}\n";


static GLuint tt_renderer_vertex_array = 0;

static size_t tt_renderer_buffer_size = 0;
static size_t tt_renderer_buffer_capacity = 0;
static TTVertex *tt_renderer_buffer_data = NULL;
static GLuint tt_renderer_buffer = 0;

static GLuint tt_renderer_shader_program = 0;
static GLint tt_renderer_camera_matrix_location = 0;
static GLint tt_renderer_position_location = 0;
static GLint tt_renderer_tex_coord_location = 0;


void tt_renderer_startup(void) {
    GLuint vertex_shader, fragment_shader;

    assert(tt_renderer_buffer_data == NULL);

    tt_renderer_buffer_capacity = 256;
    tt_renderer_buffer_size = 0;
    tt_renderer_buffer_data = (TTVertex *) malloc(
        sizeof(TTVertex) * tt_renderer_buffer_capacity
    );
    assert(tt_renderer_buffer_data != NULL);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &VERTEX_SHADER_TEXT, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &FRAGMENT_SHADER_TEXT, NULL);
    glCompileShader(fragment_shader);

    tt_renderer_shader_program = glCreateProgram();
    glAttachShader(tt_renderer_shader_program, vertex_shader);
    glAttachShader(tt_renderer_shader_program, fragment_shader);
    glLinkProgram(tt_renderer_shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    tt_renderer_camera_matrix_location = glGetUniformLocation(
        tt_renderer_shader_program, "camera_matrix"
    );
    tt_renderer_position_location = glGetAttribLocation(
        tt_renderer_shader_program, "vertex_position"
    );
    tt_renderer_tex_coord_location = glGetAttribLocation(
        tt_renderer_shader_program, "vertex_tex_coord"
    );

    /* Configure vertex array stride and size */
    glGenVertexArrays(1, &tt_renderer_vertex_array);
    glGenBuffers(1, &tt_renderer_buffer);

    glBindVertexArray(tt_renderer_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, tt_renderer_buffer);

    glEnableVertexAttribArray(tt_renderer_position_location);
    glVertexAttribPointer(
        tt_renderer_position_location, 3, GL_FLOAT, GL_FALSE,
        sizeof(TTVertex), (void*) 0
    );
    glEnableVertexAttribArray(tt_renderer_tex_coord_location);
    glVertexAttribPointer(
        tt_renderer_tex_coord_location, 2, GL_FLOAT, GL_FALSE,
        sizeof(TTVertex), (void*) (sizeof(float) * 3)
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void tt_renderer_shutdown(void) {
    assert(tt_renderer_buffer_data != NULL);

    free(tt_renderer_buffer_data);

    glDeleteVertexArrays(1, &tt_renderer_vertex_array);
    glDeleteBuffers(1, &tt_renderer_buffer);
}


void tt_renderer_push_vertex(TTVertex *vertex) {
    if (tt_renderer_buffer_size >= tt_renderer_buffer_capacity) {
        tt_renderer_buffer_capacity += tt_renderer_buffer_capacity / 2;

        tt_renderer_buffer_data = (TTVertex *) realloc(
            tt_renderer_buffer_data,
            sizeof(TTVertex) * tt_renderer_buffer_capacity
        );
        assert(tt_renderer_buffer_data == NULL);
    }

    memcpy(
        &tt_renderer_buffer_data[tt_renderer_buffer_size],
        vertex, sizeof(TTVertex)
    );

    tt_renderer_buffer_size++;
}


void tt_renderer_do_render(void) {
    float ratio;
    mat4x4 model_matrix, view_matrix, projection_matrix, mvp_matrix;

    // TODO
    ratio = 1.0;

    mat4x4_identity(model_matrix);

    vec3 eye_vector = {1.0f, -2.0f, 1.0f};
    vec3 centre_vector = {0.0f, 0.0f, 0.0f};
    vec4 up_vector = {0.0f, 0.0f, 1.0f};
    mat4x4_look_at(view_matrix, eye_vector, centre_vector, up_vector);

    mat4x4_perspective(projection_matrix, M_PI / 3, ratio, 0.1f, 100.0f);

    mat4x4_identity(mvp_matrix);
    mat4x4_mul(mvp_matrix, mvp_matrix, projection_matrix);
    mat4x4_mul(mvp_matrix, mvp_matrix, view_matrix);
    mat4x4_mul(mvp_matrix, mvp_matrix, model_matrix);

    glUseProgram(tt_renderer_shader_program);

    glUniformMatrix4fv(
        tt_renderer_camera_matrix_location,
        1, GL_FALSE, (const GLfloat*) mvp_matrix
    );

    glBindVertexArray(tt_renderer_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, tt_renderer_buffer);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(TTVertex) * tt_renderer_buffer_size, tt_renderer_buffer_data,
        GL_STREAM_DRAW
    );

    glDrawArrays(GL_TRIANGLES, 0, tt_renderer_buffer_size);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Empty the render buffer, but don't free the memory..
    tt_renderer_buffer_size = 0;
}

