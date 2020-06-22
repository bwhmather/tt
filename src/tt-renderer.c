#include "tt-renderer.h"

#include <assert.h>

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
 
static GLuint tt_renderer_buffer = 0;
static TTSpriteVertex *tt_renderer_buffer_data = NULL;
static GLuint tt_renderer_shader_program = 0;

static GLint tt_renderer_camera_matrix_location = 0;
static GLint tt_renderer_position_location = 0;
static GLint tt_renderer_tex_coord_location = 0;


void tt_renderer_startup(void) {
    GLuint vertex_shader, fragment_shader;

    assert(tt_renderer_buffer == NULL);
 
    glGenBuffers(1, &tt_renderer_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, tt_renderer_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
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
 
    tt_renderer_camera_matrixlocation = glGetUniformLocation(
        program, "camera_matrix"
    );
    tt_renderer_position_location = glGetAttribLocation(
        program, "vertex_position"
    );
    tt_renderer_tex_coord_location = glGetAttribLocation(
        program, "vertex_tex_coord"
    );
}

void tt_renderer_shutdown(void) {
    assert(tt_renderer_buffer != NULL);

}


void tt_renderer_push_vertex(TTVertex *vertex) {
    memcpy(buffer, vertex, sizeof(TTVertex));
}


void tt_renderer_do_draw(void) {
    glEnableVertexAttribArray(tt_renderer_position_location);
    glVertexAttribPointer(
        tt_renderer_position_location, 3, GL_FLOAT, GL_FALSE,
        sizeof(TTSpriteVertex), (void*) 0
    );
    glEnableVertexAttribArray(tt_renderer_tex_coord_location);
    glVertexAttribPointer(
        tt_renderer_tex_coord_location, 2, GL_FLOAT, GL_FALSE,
        sizeof(TTSpriteVertex), (void*) (sizeof(float) * 3)
    );
 
}
