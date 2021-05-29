#pragma once


void tt_resource_camera_startup();
void tt_resource_camera_shutdown();

void tt_resource_camera_set_fov(float fov);
void tt_resource_camera_set_aspect_ratio(float aspect_ratio);
void tt_resource_camera_set_near_clipping_plane(float near);
void tt_resource_camera_set_far_clipping_plane(float far);

// TODO glmc vec3
void tt_resource_camera_look_at(
    float *eye_vector, float *centre_vector, float *up_vector
);

// TODO glmc mat4
void tt_resource_camera_get_matrix(float *out);
