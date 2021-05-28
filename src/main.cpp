#include <cerrno>
#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "tt-renderer.hpp"
#include "tt-resource-camera.hpp"
#include "tt-system-sprites.hpp"

extern "C" {
#include "tt-component-brain.h"
#include "tt-component-behaviour.h"
#include "tt-component-position.h"
#include "tt-component-sprite.h"
#include "tt-component-target.h"
#include "tt-error.h"
#include "tt-entities.h"
#include "tt-system-ai.h"
#include "tt-system-behaviour.h"
}

static const struct {
    float x, y, z;
    float r, g, b;
} vertices[6] = {
    {-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
    {1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
    {-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
    {-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
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

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mods
) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static void GLAPIENTRY debug_callback(
    GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length,
    const GLchar* message, const void* userParam
) {
    fprintf(
        stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message
    );
}

int main(void) {
    GLFWwindow* window;
    GLuint vertex_buffer, vertex_array;
    GLuint vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

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

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    // gladLoadGL(glfwGetProcAddress);

    if (glewInit() != GLEW_OK) {
        exit(EXIT_FAILURE);
    }
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageControl(
        GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE
    );
    glDebugMessageCallback(debug_callback, 0);

    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity
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

    tt_entities_startup();
    tt_component_brain_startup();
    tt_component_behaviour_startup();
    tt_component_position_startup();
    tt_component_sprite_startup();
    tt_component_target_startup();

    tt_resource_camera_startup();
    tt_renderer_startup();

    tt_system_ai_startup();
    tt_system_behaviour_startup();
    tt_system_sprites_startup();

    TTEntityId tree_id = tt_entities_create();
    TTPosition *tree_position = tt_component_position_add(tree_id);
    tree_position->x = 0.4;
    tree_position->y = 0.4;

    TTSprite *tree_sprite = tt_component_sprite_add(tree_id);
    tree_sprite->grid_x = 0;
    tree_sprite->grid_y = 3;
    tree_sprite->grid_width = 2;
    tree_sprite->grid_height = 2;

    TTEntityId entity_id = tt_entities_create();

    TTPosition *position = tt_component_position_add(entity_id);
    position->x = 0.0;
    position->y = 0.0;

    TTSprite *sprite = tt_component_sprite_add(entity_id);
    sprite->grid_x = 0;
    sprite->grid_y = 0;
    sprite->grid_width = 1;
    sprite->grid_height = 1;

    tt_component_brain_set(entity_id, true);
    tt_component_target_set(entity_id, tree_id);

    tt_resource_camera_set_fov(glm::pi<float>() / 3.0f);
    tt_resource_camera_set_near_clipping_plane(0.1f);
    tt_resource_camera_set_far_clipping_plane(4.0f);
    tt_resource_camera_look_at(
        glm::vec3(1.0f, -2.0f, 1.0f),  // Eye vector.
        glm::vec3(0.0f, 0.0f, 0.0f),  // Centre vector.
        glm::vec3(0.0f, 0.0f, 1.0f)  // Up vector.
    );

    while (!glfwWindowShouldClose(window)) {
        float aspect_ratio;
        int width, height;

        glfwGetFramebufferSize(window, &width, &height);
        aspect_ratio = (float) width / (float) height;
        tt_resource_camera_set_aspect_ratio(aspect_ratio);

        glm::mat4 camera_matrix = tt_resource_camera_get_matrix();

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        glUniformMatrix4fv(
            mvp_location, 1, GL_FALSE, glm::value_ptr(camera_matrix)
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

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

