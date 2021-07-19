#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "bt.h"
#include "tt-component-brain.h"
#include "tt-component-behaviour.h"
#include "tt-component-behaviour-stack.h"
#include "tt-component-harvestable.h"
#include "tt-component-position.h"
#include "tt-component-sprite.h"
#include "tt-component-target.h"
#include "tt-component-wood.h"
#include "tt-error.h"
#include "tt-entities.h"
#include "tt-renderer.h"
#include "tt-resource-camera.h"
#include "tt-system-ai.h"
#include "tt-system-behaviour.h"
#include "tt-system-sprites.h"


static const struct {
    float x, y, z;
    float r, g, b;
} vertices[6] = {
    {-1.0f, -1.0f, 0.0f, 0.2f, 0.6f, 0.1f},
    {1.0f, -1.0f, 0.0f, 0.2f, 0.6f, 0.1f},
    {1.0f, 1.0f, 0.0f, 0.2f, 0.6f, 0.1f},
    {1.0f, 1.0f, 0.0f, 0.2f, 0.6f, 0.1f},
    {-1.0f, 1.0f, 0.0f, 0.2f, 0.6f, 0.1f},
    {-1.0f, -1.0f, 0.0f, 0.2f, 0.6f, 0.1f},
};

static const char* vertex_shader_text =
    "#version 110\n"
    "uniform mat4 MVP;\n"
    "attribute vec3 vCol;\n"
    "attribute vec3 vPos;\n"
    "varying vec3 color;\n"
    "void main() {\n"
    "    gl_Position = MVP * vec4(vPos, 1.0);\n"
    "    color = vCol;\n"
    "}\n";

static const char* fragment_shader_text =
    "#version 110\n"
    "varying vec3 color;\n"
    "void main() {\n"
    "    gl_FragColor = vec4(color, 1.0);\n"
    "}\n";


static void tt_main_glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW ERROR: %s (%i)\n", description, error);
}

static void tt_main_glfw_key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mods
) {
    (void) scancode;
    (void) mods;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}


static const char *tt_gl_error_source_str(GLenum source) {
    switch(source) {
      case GL_DEBUG_SOURCE_API:
        return "GL API";
      case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        return "GL Window System";
      case GL_DEBUG_SOURCE_SHADER_COMPILER:
        return "GL Shader Compiler";
      case GL_DEBUG_SOURCE_THIRD_PARTY:
        return "GL Third-Party";
      case GL_DEBUG_SOURCE_APPLICATION:
        return "GL Application";
      default:
        return "GL";
    }
}

static const char *tt_gl_error_level_str(GLenum level) {
    switch (level) {
      case GL_DEBUG_SEVERITY_LOW:
        return "DEBUG";
      case GL_DEBUG_SEVERITY_MEDIUM:
        return "INFO";
      case GL_DEBUG_SEVERITY_HIGH:
        return "ERROR";
      default:
        return "ERROR";
    }
}

static const char *tt_gl_error_type_str(GLenum type) {
    switch (type) {
      case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        return "deprecated behaviour: ";
      case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        return "undefined behaviour: ";
      case GL_DEBUG_TYPE_PORTABILITY:
        return "portability: ";
      case GL_DEBUG_TYPE_PERFORMANCE:
        return "performance: ";
      case GL_DEBUG_TYPE_MARKER:
        return "marker: ";
      case GL_DEBUG_TYPE_PUSH_GROUP:
        return "push group: ";
      case GL_DEBUG_TYPE_POP_GROUP:
        return "pop group: ";
      default:
        return "";
    }
}

static void GLAPIENTRY tt_main_gl_debug_callback(
    GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length,
    const GLchar* message, const void* userParam
) {
    (void) id;
    (void) length;
    (void) userParam;

    fprintf(
        stderr, "%s: %s:%s %s\n",
        tt_gl_error_source_str(source),
        tt_gl_error_level_str(severity),
        tt_gl_error_type_str(type),
        message
    );
}

static TTEntityId spawn_tree(void) {
    TTEntityId tree_id = tt_entities_create();
    TTPosition *tree_position = tt_component_position_add(tree_id);
    tree_position->x = 2 * ((double) rand())/((double) RAND_MAX) - 1;;
    tree_position->y = 2 * ((double) rand())/((double) RAND_MAX) - 1;;

    TTSprite *tree_sprite = tt_component_sprite_add(tree_id);
    tree_sprite->grid_x = 0;
    tree_sprite->grid_y = 3;
    tree_sprite->grid_width = 2;
    tree_sprite->grid_height = 2;

    tt_component_harvestable_set(tree_id, true);
    tt_component_wood_set(tree_id, 200);

    return tree_id;
}


static TTEntityId spawn_villager(void) {
    TTEntityId entity_id = tt_entities_create();

    TTPosition *position = tt_component_position_add(entity_id);
    position->x = 2 * ((double) rand())/((double) RAND_MAX) - 1;;
    position->y = 2 * ((double) rand())/((double) RAND_MAX) - 1;;

    TTSprite *sprite = tt_component_sprite_add(entity_id);
    sprite->grid_x = 0;
    sprite->grid_y = 0;
    sprite->grid_width = 1;
    sprite->grid_height = 1;

    tt_component_brain_set(entity_id, true);

    return entity_id;
}


int main(void) {
    GLFWwindow* window;
    GLuint vertex_buffer, vertex_array;
    GLuint vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(tt_main_glfw_error_callback);

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    errno = 0;
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, tt_main_glfw_key_callback);

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        exit(EXIT_FAILURE);
    }
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageControl(
        GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE
    );
    glDebugMessageCallback(tt_main_gl_debug_callback, 0);
    tt_abort_if_gl_error("failed to configure debug output");

    glfwSwapInterval(1);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    tt_abort_if_gl_error("failed to compile terrain shader program");

    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    glGenVertexArrays(1, &vertex_array);
    glGenBuffers(1, &vertex_buffer);

    glBindVertexArray(vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(
        vpos_location, 3, GL_FLOAT, GL_FALSE,
        sizeof(vertices[0]), (void*) 0
    );
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(
        vcol_location, 3, GL_FLOAT, GL_FALSE,
        sizeof(vertices[0]), (void*) (sizeof(float) * 3)
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    tt_abort_if_gl_error("failed to bind terrain shader objects");

    tt_entities_startup();
    tt_component_brain_startup();
    tt_component_behaviour_startup();
    tt_component_behaviour_stack_startup();
    tt_component_harvestable_startup();
    tt_component_position_startup();
    tt_component_sprite_startup();
    tt_component_target_startup();
    tt_component_wood_startup();

    tt_resource_camera_startup();
    tt_renderer_startup();

    tt_system_ai_startup();
    tt_system_behaviour_startup();
    tt_system_sprites_startup();

    for (int i = 0; i< 20; i++) {
        spawn_tree();
    }

    for (int i = 0; i< 20; i++) {
        spawn_villager();
    }

    tt_resource_camera_set_fov(M_PI / 3.0f);
    tt_resource_camera_set_near_clipping_plane(0.1f);
    tt_resource_camera_set_far_clipping_plane(4.0f);

    vec3 eye_vector = { 1.0f, -2.0f, 1.0f };
    vec3 centre_vector = { 0.0f, 0.0f, 0.0f };
    vec3 up_vector = { 0.0f, 0.0f, 1.0f };
    tt_resource_camera_look_at(eye_vector, centre_vector, up_vector);

    while (!glfwWindowShouldClose(window)) {
        float aspect_ratio;
        int width, height;

        glfwGetFramebufferSize(window, &width, &height);
        aspect_ratio = (float) width / (float) height;
        tt_resource_camera_set_aspect_ratio(aspect_ratio);

        mat4 camera_matrix;
        tt_resource_camera_get_matrix(camera_matrix);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        glUniformMatrix4fv(
            mvp_location, 1, GL_FALSE, (float *) camera_matrix
        );

        glBindVertexArray(vertex_array);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

        glBufferData(
            GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW
        );

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        tt_entities_maintain();

        tt_system_ai_run();
        tt_system_behaviour_run();

        tt_system_sprites_run();
        tt_renderer_do_render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    tt_renderer_shutdown();
    tt_resource_camera_shutdown();

    tt_system_sprites_shutdown();
    tt_system_behaviour_shutdown();
    tt_system_ai_shutdown();

    tt_component_wood_shutdown();
    tt_component_target_shutdown();
    tt_component_sprite_shutdown();
    tt_component_position_shutdown();
    tt_component_harvestable_shutdown();
    tt_component_behaviour_stack_shutdown();
    tt_component_behaviour_shutdown();
    tt_component_brain_shutdown();
    tt_entities_shutdown();

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

