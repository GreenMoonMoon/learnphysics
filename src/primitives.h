//
// Created by josue on 2024-02-26.
//

#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "cglm/cglm.h"

typedef struct RayPrimitive {
    vec3 origin;
    vec3 direction;
} RayPrimitive;

typedef struct PlanePrimitive {
    vec3 normal;
    float distance;
} PlanePrimitive;

#endif //PRIMITIVES_H
