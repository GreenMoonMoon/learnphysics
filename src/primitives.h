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
typedef struct Plane {
    vec3 normal;
    float distance;
} Plane;

// BOUNDING VOLUME
/// \brief Axis-Aligned Bounding Box composed of the lower bottom-near point and the high top-far point.
typedef struct AABB {
    vec3 min;
    vec3 max;
} AABB;

AABB aabb_union(AABB a, AABB b);

#endif //PRIMITIVES_H
