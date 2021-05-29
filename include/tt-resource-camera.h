#pragma once

#include <cglm/cglm.h>


void tt_resource_camera_startup();
void tt_resource_camera_shutdown();

void tt_resource_camera_set_fov(float fov);
void tt_resource_camera_set_aspect_ratio(float aspect_ratio);
void tt_resource_camera_set_near_clipping_plane(float near);
void tt_resource_camera_set_far_clipping_plane(float far);

void tt_resource_camera_look_at(
    vec3 eye_vector, vec3 centre_vector, vec3 up_vector
);

void tt_resource_camera_get_matrix(mat4 out);
