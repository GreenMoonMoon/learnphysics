//
// Created by moonmoon on 23/01/24.
//

#ifndef LEARNPHYSICS_RIGIDBODY_H
#define LEARNPHYSICS_RIGIDBODY_H

#include "cglm/cglm.h"

typedef struct Rigidbody {
    vec3 velocity;
    mat4 transform;
} Rigidbody;

void integrate_velocity(Rigidbody *rigidbody, float delta_time);

#endif //LEARNPHYSICS_RIGIDBODY_H
