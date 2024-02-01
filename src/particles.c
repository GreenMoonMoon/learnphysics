//
// Created by josue on 2024-01-30.
//

#include "particles.h"

void particle_integrate(Particle* particle, const float delta) {
    // TODO: extract somewhere else. For example to the particle system or individual bodies.
    constexpr float damping = 0.98f;

    // Integrate Position
    vec3 velocity_delta;
    glm_vec3_scale(particle->velocity, delta, velocity_delta);
    glm_vec3_add(particle->position, velocity_delta, particle->position);

    // Integrate Velocity
    vec3 acceleration_delta;
    glm_vec3_scale(particle->acceleration, delta, acceleration_delta);
    glm_vec3_add(particle->velocity, acceleration_delta, particle->velocity);

    // Add drag
    glm_vec3_scale(particle->velocity, powf(damping, delta), particle->velocity);
}

void particle_apply_forces(Particle* particle, vec3 total_force) {
    vec3 force;
    glm_vec3_scale(total_force, particle->inverse_mass, force);
    glm_vec3_add(particle->acceleration, force, particle->acceleration);
}
