#include "tt-resource-camera.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace tt {
namespace resource_camera {

namespace state {
    static float fov;
    static float aspect_ratio;

    static float near_clipping_plane;
    static float far_clipping_plane;

    static glm::vec3 eye_vector;
    static glm::vec3 centre_vector;
    static glm::vec3 up_vector;
}  /* namespace state */

void startup() {
    state::fov = glm::pi<float>() / 3.0f;
    state::aspect_ratio = 1.0f;

    state::near_clipping_plane = 1.0f;
    state::far_clipping_plane = 100.0f;

    state::eye_vector = glm::vec3(0.0f, -1.0f, 0.0f);
    state::centre_vector = glm::vec3(0.0f, 0.0f, 0.0f);
    state::up_vector = glm::vec3(0.0, 0.0, 1.0f);
}

void shutdown() {}

void set_fov(float fov) {
    state::fov = fov;
}

void set_aspect_ratio(float aspect_ratio) {
    state::aspect_ratio = aspect_ratio;
}

void set_near_clipping_plane(float near) {
    state::near_clipping_plane = near;
}

void set_far_clipping_plane(float far) {
    state::far_clipping_plane = far;
}

void look_at(
    glm::vec3 eye_vector, glm::vec3 centre_vector, glm::vec3 up_vector
) {
    state::eye_vector = eye_vector;
    state::centre_vector = centre_vector;
    state::up_vector = up_vector;
}

glm::mat4 get_matrix(void) {
    glm::mat4 model_matrix = glm::identity<glm::mat4>();

    glm::mat4 view_matrix = glm::lookAt(
        state::eye_vector, state::centre_vector, state::up_vector
    );

    glm::mat4 projection_matrix = glm::perspective(
        state::fov, state::aspect_ratio,
        state::near_clipping_plane, state::far_clipping_plane
    );

    return projection_matrix * view_matrix * model_matrix;
}

}  /* namespace resource_camera */
}  /* namespace tt */
