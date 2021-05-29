extern "C" {
#include "tt-resource-camera.h"
}

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

extern "C" {
#include "tt-error.h"
}

namespace state {
    static bool initialised = false;

    static float fov;
    static float aspect_ratio;

    static float near_clipping_plane;
    static float far_clipping_plane;

    static glm::vec3 eye_vector;
    static glm::vec3 centre_vector;
    static glm::vec3 up_vector;
}

extern "C" void tt_resource_camera_startup() {
    tt_assert(state::initialised == false);

    state::fov = glm::pi<float>() / 3.0f;
    state::aspect_ratio = 1.0f;

    state::near_clipping_plane = 1.0f;
    state::far_clipping_plane = 100.0f;

    state::eye_vector = glm::vec3(0.0f, -1.0f, 0.0f);
    state::centre_vector = glm::vec3(0.0f, 0.0f, 0.0f);
    state::up_vector = glm::vec3(0.0, 0.0, 1.0f);

    state::initialised = true;
}

extern "C" void tt_resource_camera_shutdown() {
    tt_assert(state::initialised == true);

    state::initialised = false;
}

extern "C" void tt_resource_camera_set_fov(float fov) {
    tt_assert(state::initialised == true);

    state::fov = fov;
}

extern "C" void tt_resource_camera_set_aspect_ratio(float aspect_ratio) {
    tt_assert(state::initialised == true);

    state::aspect_ratio = aspect_ratio;
}

extern "C" void tt_resource_camera_set_near_clipping_plane(float near) {
    tt_assert(state::initialised == true);

    state::near_clipping_plane = near;
}

extern "C" void tt_resource_camera_set_far_clipping_plane(float far) {
    tt_assert(state::initialised == true);

    state::far_clipping_plane = far;
}

extern "C" void tt_resource_camera_look_at(
    float *eye_vector, float *centre_vector, float *up_vector
) {
    tt_assert(state::initialised == true);

    state::eye_vector = glm::make_vec3(eye_vector);
    state::centre_vector = glm::make_vec3(centre_vector);
    state::up_vector = glm::make_vec3(up_vector);
}

extern "C" void tt_resource_camera_get_matrix(float *out) {
    tt_assert(state::initialised == true);

    glm::mat4 model_matrix = glm::identity<glm::mat4>();

    glm::mat4 view_matrix = glm::lookAt(
        state::eye_vector, state::centre_vector, state::up_vector
    );

    glm::mat4 projection_matrix = glm::perspective(
        state::fov, state::aspect_ratio,
        state::near_clipping_plane, state::far_clipping_plane
    );

    memcpy(
        out, glm::value_ptr(projection_matrix * view_matrix * model_matrix),
        16 * sizeof(float)
    );
}
