//
// Created by josue on 2024-01-24.
//

#include "collision.h"
#include "cglm/cglm.h"


bool get_sphere_plane_collision(SphereCollider a, vec3 position, vec4 plane, Collision* out_collision) {
    return false;
}

bool get_sphere_sphere_collision(SphereCollider a, SphereCollider b, Collision *out_collision) {
    return false;
}

bool get_ray_plane_intersection(RayPrimitive ray, Plane plane, vec3 result) {
    float distance = (plane.distance - glm_vec3_dot(plane.normal, ray.origin)) / glm_vec3_dot(plane.normal, ray.direction);

    glm_vec3_scale(ray.direction, distance, result);
    glm_vec3_add(ray.origin, result, result);
    return true;
}

bool get_collision_ray_aabb(RayPrimitive ray, AABB aabb, Collision* collisions) {
    return false;
}
