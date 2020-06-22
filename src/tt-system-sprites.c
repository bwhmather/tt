#include "tt-renderer-sprites.h"

#include "tt-entities.h"
#include "tt-component-position.h"
#include "tt-component-sprite.h"

#include <assert.h>

 
typedef struct {
    float x, y, z,
    float u, v
} TTSpriteVertex;

 
static const char* VERTEX_SHADER_TEXT =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec3 vPos;\n"
"varying vec3 color;\n"
"void main() {\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
"    color = vCol;\n"
"}\n";
 
static const char* FRAGMENT_SHADER_TEXT =
"#version 110\n"
"varying vec3 color;\n"
"void main() {\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";
 
static GLuint tt_renderer_sprites_buffer = 0;
static TTSpriteVertex *tt_renderer_sprites_buffer_data = NULL;
static GLuint tt_renderer_sprites_shader_program = 0;

static GLint tt_renderer_sprites_camera_matrix_location = 0;
static GLint tt_renderer_sprites_position_location = 0;
static GLint tt_renderer_sprites_tex_coord_location = 0;


void tt_renderer_sprites_startup(void) {
    GLuint vertex_shader, fragment_shader;

    assert(tt_renderer_sprites_buffer == NULL);
 
    glGenBuffers(1, &tt_renderer_sprites_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, tt_renderer_sprites_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &VERTEX_SHADER_TEXT, NULL);
    glCompileShader(vertex_shader);
 
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &FRAGMENT_SHADER_TEXT, NULL);
    glCompileShader(fragment_shader);
 
    tt_renderer_sprites_shader_program = glCreateProgram();
    glAttachShader(tt_renderer_sprites_shader_program, vertex_shader);
    glAttachShader(tt_renderer_sprites_shader_program, fragment_shader);
    glLinkProgram(tt_renderer_sprites_shader_program);
 
    tt_renderer_sprites_camera_matrixlocation = glGetUniformLocation(
        program, "camera_matrix"
    );
    tt_renderer_sprites_position_location = glGetAttribLocation(
        program, "vertex_position"
    );
    tt_renderer_sprites_tex_coord_location = glGetAttribLocation(
        program, "vertex_tex_coord"
    );
 

}

void tt_renderer_sprites_shutdown(void) {
    assert(tt_renderer_sprites_buffer != NULL);

}

void tt_renderer_sprites_run(void) {

    glEnableVertexAttribArray(tt_renderer_sprites_position_location);
    glVertexAttribPointer(
        tt_renderer_sprites_position_location, 3, GL_FLOAT, GL_FALSE,
        sizeof(TTSpriteVertex), (void*) 0
    );
    glEnableVertexAttribArray(tt_renderer_sprites_tex_coord_location);
    glVertexAttribPointer(
        tt_renderer_sprites_tex_coord_location, 2, GL_FLOAT, GL_FALSE,
        sizeof(TTSpriteVertex), (void*) (sizeof(float) * 3)
    );
 
}






void tt_component_sprite_startup(void) {
    assert(tt_component_sprite_storage == NULL);

    tt_component_sprite_storage = tt_storage_vector_new(sizeof(TTSprite));
}

void tt_component_sprite_shutdown(void) {
    assert(tt_component_sprite_storage != NULL);

    tt_storage_vector_free(tt_component_sprite_storage);
    tt_component_sprite_storage = NULL;
}

TTSprite *tt_component_sprite_add(TTEntityId entity) {
    assert(tt_component_sprite_storage != NULL);

    return (TTSprite *) tt_storage_vector_add(
        tt_component_sprite_storage, entity
    );
}

TTSprite *tt_component_sprite_get(TTEntityId entity) {
    assert(tt_component_sprite_storage != NULL);

    return (TTSprite *) tt_storage_vector_get(
        tt_component_sprite_storage, entity
    );
}

void tt_component_sprite_remove(TTEntityId entity) {
    assert(tt_component_sprite_storage != NULL);

    tt_storage_vector_remove(tt_component_sprite_storage, entity);
}



