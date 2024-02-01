//
// Created by josue on 2024-01-30.
//

#include "particles.h"

void particle_integrate(Particle* particle, const float damping, const float delta) {
    // Integrate Position
    vec3 velocity_delta;
    glm_vec3_scale(particle->velocity, delta, velocity_delta);
    glm_vec3_add(particle->position, velocity_delta, particle->position);

    // Integrate Velocity
    vec3 acceleration;
    glm_vec3_scale(particle->force_sum, particle->inverse_mass, acceleration);
    glm_vec3_scale(acceleration, delta, acceleration);
    glm_vec3_add(particle->velocity, acceleration, particle->velocity);
    particle->force_sum[0] = 0.0f;
    particle->force_sum[1] = 0.0f;
    particle->force_sum[2] = 0.0f;

    // Add drag
    glm_vec3_scale(particle->velocity, powf(damping, delta), particle->velocity);
}

void particle_apply_forces(Particle* particle, vec3 force) {
    glm_vec3_add(particle->force_sum, force, particle->force_sum);
}
