//
// Created by josue on 2024-02-26.
//

#include "primitives.h"
#include "cglm/cglm.h"

AABB aabb_union(AABB a, AABB b) {
    AABB result;
    glm_vec3_minv(a.min, b.min, result.min);
    glm_vec3_maxv(a.max, b.max, result.max);

    return result;
}
