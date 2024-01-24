//
// Created by josue on 2024-01-24.
//

#include "collision.h"

bool get_sphere_plane_collision(SphereCollider a, vec3 position, vec4 plane, Collision* out_collision) {
    // -(dot(ro,p.xyz)+p.w)/dot(rd,p.xyz);
    // http://iquilezles.org/articles/intersectors/
    vec3 sphere_position;
    glm_vec2_add(a.position, position, sphere_position);
    vec3 plane_sphere_direction;
    glm_vec3_sub(sphere_position, plane, plane_sphere_direction);
    out_collision->depth = (a.radius - glm_dot(sphere_position, plane) + plane[3] / glm_dot(plane_sphere_direction, plane));

    return out_collision->depth >= 0.0f;
}

bool get_sphere_sphere_collision(SphereCollider a, SphereCollider b, Collision *out_collision) {

}
