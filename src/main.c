#include "linmath.h"

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

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



void render(void) {
    TTVertex vertex;
    vertex.x = x;
    vertex.y = y;
    vertex.z = z;
    vertex.u = u;
    vertex.v = v;
    tt_renderer_push_vertex(&vertex)
}


int main(void) {
    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
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

    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

    while (!glfwWindowShouldClose(window)) {
        float ratio;
        int width, height;
        mat4x4 model_matrix, view_matrix, projection_matrix, mvp_matrix;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        mat4x4_identity(model_matrix);

        vec3 eye_vector = {-2.0f, 0.0f, 1.0f};
        vec3 centre_vector = {0.0f, 0.0f, 0.0f};
        vec4 up_vector = {0.0f, 0.0f, 1.0f};
        mat4x4_look_at(view_matrix, eye_vector, centre_vector, up_vector);

        mat4x4_perspective(projection_matrix, M_PI / 3, ratio, 0.1f, 100.0f);

        mat4x4_identity(mvp_matrix);
        mat4x4_mul(mvp_matrix, mvp_matrix, projection_matrix);
        mat4x4_mul(mvp_matrix, mvp_matrix, view_matrix);
        mat4x4_mul(mvp_matrix, mvp_matrix, model_matrix);

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp_matrix);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

