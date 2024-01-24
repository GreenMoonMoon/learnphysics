//
// Created by moonmoon on 23/01/24.
//

#include "rigidbody.h"


void integrate_linear(Rigidbody* rigidbody, vec3 forces, float delta_time) {
    vec3 acceleration;
    // glm_vec3_scale(forces, rigidbody.inverse_mass, scaled_velocity);
    glm_vec3_scale(forces, 1.0f * delta_time * delta_time, acceleration); // assume a mass of 1
    glm_vec3_add(rigidbody->velocity, acceleration, rigidbody->velocity);

    vec3 scaled_velocity;
    glm_vec3_scale(rigidbody->velocity, delta_time, scaled_velocity);
    glm_translate(rigidbody->transform, scaled_velocity);
}
