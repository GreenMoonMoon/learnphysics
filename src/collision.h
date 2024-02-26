//
// Created by josue on 2024-01-24.
//

#ifndef COLLISION_H
#define COLLISION_H

#include "cglm/cglm.h"
#include "primitives.h"
#include "raymath.h"

typedef struct SphereCollider {
    vec3 position;
    float radius;
} SphereCollider;

typedef struct Collision {
    vec3 start;
    vec3 normal;
    float depth;
} Collision;

bool get_sphere_plane_collision(SphereCollider a, vec3 position, vec4 plane, Collision *out_collision);

bool get_sphere_sphere_collision(SphereCollider a, SphereCollider b, Collision *out_collision);

Vector3 get_ray_plane_intersection(RayPrimitive ray, PlanePrimitive plane);
#endif //COLLISION_H
