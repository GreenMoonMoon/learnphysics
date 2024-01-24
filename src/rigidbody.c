//
// Created by moonmoon on 23/01/24.
//

#include "rigidbody.h"

void integrate_velocity(Rigidbody* rigidbody, float delta_time) {
    vec3 scaled_velocity;
    glm_vec3_scale(rigidbody->velocity, delta_time, scaled_velocity);
    glm_translate(rigidbody->transform, scaled_velocity);
}
