#include "tt-resource-camera.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace tt {
namespace resource_camera {

namespace detail {
static float fov;
static float aspect_ratio;

static float near_clipping_plane;
static float far_clipping_plane;

static glm::vec3 eye_vector;
static glm::vec3 centre_vector;
static glm::vec3 up_vector;
}  /* namespace detail */

void startup() {
    detail::fov = glm::pi<float>() / 3.0f;
    detail::aspect_ratio = 1.0f;

    detail::near_clipping_plane = 1.0f;
    detail::far_clipping_plane = 100.0f;

    detail::eye_vector = glm::vec3(0.0f, -1.0f, 0.0f);
    detail::centre_vector = glm::vec3(0.0f, 0.0f, 0.0f);
    detail::up_vector = glm::vec3(0.0, 0.0, 1.0f);
}

void shutdown() {}

void set_fov(float fov) {
    detail::fov = fov;
}

void set_aspect_ratio(float aspect_ratio) {
    detail::aspect_ratio = aspect_ratio;
}

void set_near_clipping_plane(float near) {
    detail::near_clipping_plane = near;
}

void set_far_clipping_plane(float far) {
    detail::far_clipping_plane = far;
}

void look_at(
    glm::vec3 eye_vector, glm::vec3 centre_vector, glm::vec3 up_vector
) {
    detail::eye_vector = eye_vector;
    detail::centre_vector = centre_vector;
    detail::up_vector = up_vector;
}

glm::mat4 get_matrix(void) {
    glm::mat4 model_matrix = glm::identity<glm::mat4>();

    glm::mat4 view_matrix = glm::lookAt(
        detail::eye_vector, detail::centre_vector, detail::up_vector
    );

    glm::mat4 projection_matrix = glm::perspective(
        detail::fov, detail::aspect_ratio,
        detail::near_clipping_plane, detail::far_clipping_plane
    );

    return projection_matrix * view_matrix * model_matrix;
}

}  /* namespace resource_camera */
}  /* namespace tt */
