#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

void tt_resource_camera_startup();
void tt_resource_camera_shutdown();

void tt_resource_camera_set_fov(float fov);
void tt_resource_camera_set_aspect_ratio(float aspect_ratio);
void tt_resource_camera_set_near_clipping_plane(float near);
void tt_resource_camera_set_far_clipping_plane(float far);

void tt_resource_camera_look_at(
    glm::vec3 eye_vector, glm::vec3 centre_vector, glm::vec3 up_vector
);

glm::mat4 tt_resource_camera_get_matrix(void);
