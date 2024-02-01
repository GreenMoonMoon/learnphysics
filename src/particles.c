//
// Created by josue on 2024-01-30.
//

#include "particles.h"

void particle_apply_forces(Particle* particle, vec3 force_sum, float delta) {
    vec3 acceleration;
    glm_vec3_scale(force_sum, particle->inverse_mass, acceleration);
    glm_vec3_scale(acceleration, delta, acceleration);
    glm_vec3_add(particle->velocity, acceleration, particle->velocity);
}

void particle_integrate(Particle* particle, const float damping, const float delta) {
    // Integrate Position
    vec3 velocity_delta;
    glm_vec3_scale(particle->velocity, delta, velocity_delta);
    glm_vec3_add(particle->position, velocity_delta, particle->position);

    // Add drag
    glm_vec3_scale(particle->velocity, powf(damping, delta), particle->velocity);
}

