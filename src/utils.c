//
// Created by josue on 2024-01-25.
//

#include "utils.h"

void reflect(vec3 vector, vec3 normal) {
    // I - 2.0 * dot(N, I) * N
    // where I is a vector and N the surface normal
    vec3 n;
    glm_vec3_scale(normal, 2.0f * glm_dot(normal, vector), n);
    glm_vec3_sub(vector, n, vector);
}
