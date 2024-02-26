//
// Created by josue on 2024-02-26.
//

#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "cglm/cglm.h"

/// \brief Simple ray composed of an origin and a direction used for intersection tests
typedef struct RayPrimitive {
    vec3 origin;
    vec3 direction;
} RayPrimitive;

/// \brief A plane representation in the form of (n⋅X)=d.
typedef struct PlanePrimitive {
    vec3 normal;
    float distance;
} PlanePrimitive;

// COLLIDERS
typedef struct AABB {
    vec3 min;
    vec3 max;
} AABB;

#endif //PRIMITIVES_H
