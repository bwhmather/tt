#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace tt {
namespace resource_camera {

void startup();
void shutdown();

void set_fov(float fov);
void set_aspect_ratio(float aspect_ratio);
void set_near_clipping_plane(float near);
void set_far_clipping_plane(float far);

void look_at(
    glm::vec3 eye_vector, glm::vec3 centre_vector, glm::vec3 up_vector
);

glm::mat4 get_matrix(void);

}  /* namespace tt */
}  /* namespace resource_camera */
