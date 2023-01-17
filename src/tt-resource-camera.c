#include "tt-resource-camera.h"

#include <math.h>
#include <stdbool.h>

#include <cglm/cglm.h>

#include "tt-error.h"

static struct TTResourceCameraState {
    bool initialised;

    float fov;
    float aspect_ratio;

    float near_clipping_plane;
    float far_clipping_plane;

    vec3 eye_vector;
    vec3 centre_vector;
    vec3 up_vector;
} state = {.initialised = false};

void
tt_resource_camera_startup(void) {
    tt_assert(state.initialised == false);

    state.fov = M_PI / 3.0f;
    state.aspect_ratio = 1.0f;

    state.near_clipping_plane = 1.0f;
    state.far_clipping_plane = 100.0f;

    glm_vec3_copy((vec3){0.0f, -1.0f, 0.0f}, state.eye_vector);
    glm_vec3_copy((vec3){0.0f, 0.0f, 0.0f}, state.centre_vector);
    glm_vec3_copy((vec3){0.0, 0.0, 1.0f}, state.up_vector);

    state.initialised = true;
}

void
tt_resource_camera_shutdown(void) {
    tt_assert(state.initialised == true);

    state.initialised = false;
}

void
tt_resource_camera_set_fov(float fov) {
    tt_assert(state.initialised == true);

    state.fov = fov;
}

void
tt_resource_camera_set_aspect_ratio(float aspect_ratio) {
    tt_assert(state.initialised == true);

    state.aspect_ratio = aspect_ratio;
}

void
tt_resource_camera_set_near_clipping_plane(float near) {
    tt_assert(state.initialised == true);

    state.near_clipping_plane = near;
}

void
tt_resource_camera_set_far_clipping_plane(float far) {
    tt_assert(state.initialised == true);

    state.far_clipping_plane = far;
}

void
tt_resource_camera_look_at(
    vec3 eye_vector, vec3 centre_vector, vec3 up_vector
) {
    tt_assert(state.initialised == true);

    glm_vec3_copy(eye_vector, state.eye_vector);
    glm_vec3_copy(centre_vector, state.centre_vector);
    glm_vec3_copy(up_vector, state.up_vector);
}

void
tt_resource_camera_get_matrix(mat4 out) {
    tt_assert(state.initialised == true);

    mat4 model_matrix;
    glm_mat4_identity(model_matrix);

    mat4 view_matrix;
    glm_lookat(
        state.eye_vector, state.centre_vector, state.up_vector, view_matrix
    );

    mat4 projection_matrix;
    glm_perspective(
        state.fov, state.aspect_ratio, state.near_clipping_plane,
        state.far_clipping_plane, projection_matrix
    );

    glm_mat4_identity(out);
    glm_mat4_mul(out, projection_matrix, out);
    glm_mat4_mul(out, view_matrix, out);
    glm_mat4_mul(out, model_matrix, out);
}
