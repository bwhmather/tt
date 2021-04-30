#include "tt-renderer.hpp"

#include <cassert>
#include <cstdlib>
#include <cstring>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#include "tt-resource-camera.hpp"
#include "tt-texture.hpp"

namespace tt {
namespace renderer {

namespace detail {

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


static GLuint vertex_array = 0;

static size_t buffer_size = 0;
static size_t buffer_capacity = 0;
static vertex *buffer_data = NULL;
static GLuint buffer = 0;

static GLuint shader_program = 0;
static GLuint spritesheet = 0;

static GLint camera_matrix_location = 0;
static GLint spritesheet_location = 0;
static GLint position_location = 0;
static GLint tex_coord_location = 0;

} /* namespace detail */

void startup(void) {
    GLuint vertex_shader, fragment_shader;

    assert(detail::buffer_data == NULL);

    detail::buffer_capacity = 256;
    detail::buffer_size = 0;
    detail::buffer_data = (vertex *) malloc(
        sizeof(vertex) * detail::buffer_capacity
    );
    assert(detail::buffer_data != NULL);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &detail::VERTEX_SHADER_TEXT, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &detail::FRAGMENT_SHADER_TEXT, NULL);
    glCompileShader(fragment_shader);

    detail::shader_program = glCreateProgram();
    glAttachShader(detail::shader_program, vertex_shader);
    glAttachShader(detail::shader_program, fragment_shader);
    glLinkProgram(detail::shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    detail::camera_matrix_location = glGetUniformLocation(
        detail::shader_program, "camera_matrix"
    );
    detail::spritesheet_location = glGetAttribLocation(
        detail::shader_program, "spritesheet"
    );
    detail::position_location = glGetAttribLocation(
        detail::shader_program, "vertex_position"
    );
    detail::tex_coord_location = glGetAttribLocation(
        detail::shader_program, "vertex_tex_coord"
    );

    /* Configure vertex array stride and size */
    glGenVertexArrays(1, &detail::vertex_array);
    glGenBuffers(1, &detail::buffer);

    glBindVertexArray(detail::vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, detail::buffer);

    glEnableVertexAttribArray(detail::position_location);
    glVertexAttribPointer(
        detail::position_location, 3, GL_FLOAT, GL_FALSE,
        sizeof(vertex), (void*) 0
    );
    glEnableVertexAttribArray(detail::tex_coord_location);
    glVertexAttribPointer(
        detail::tex_coord_location, 2, GL_FLOAT, GL_FALSE,
        sizeof(vertex), (void*) (sizeof(float) * 3)
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    detail::spritesheet = tt_load_texture("spritesheet.png");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void shutdown(void) {
    assert(detail::buffer_data != NULL);

    free(detail::buffer_data);

    glDeleteVertexArrays(1, &detail::vertex_array);
    glDeleteBuffers(1, &detail::buffer);
}


void push_vertex(vertex *v) {
    if (detail::buffer_size >= detail::buffer_capacity) {
        detail::buffer_capacity += detail::buffer_capacity / 2;

        detail::buffer_data = (vertex *) realloc(
            detail::buffer_data,
            sizeof(vertex) * detail::buffer_capacity
        );
        assert(detail::buffer_data == NULL);
    }

    memcpy(
        &detail::buffer_data[detail::buffer_size],
        v, sizeof(vertex)
    );

    detail::buffer_size++;
}


void do_render(void) {
    glm::mat4 camera_matrix = tt_camera_get_matrix();

    glUseProgram(detail::shader_program);

    glUniformMatrix4fv(
        detail::camera_matrix_location,
        1, GL_FALSE, glm::value_ptr(camera_matrix)
    );

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, detail::spritesheet);
    glUniform1i(detail::spritesheet_location, 0);

    glBindVertexArray(detail::vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, detail::buffer);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertex) * detail::buffer_size, detail::buffer_data,
        GL_STREAM_DRAW
    );

    glDrawArrays(GL_TRIANGLES, 0, detail::buffer_size);

    glBindTexture(GL_TEXTURE_2D, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Empty the render buffer, but don't free the memory..
    detail::buffer_size = 0;
}

} /* namespace renderer */
} /* namespace tt */
