//
// Created by josue on 2024-01-30.
//

#ifndef PARTICLES_H
#define PARTICLES_H

#include "cglm/cglm.h"

typedef struct Particle {
    vec3 position;
    vec3 velocity;
    float inverse_mass;
} Particle;

void particle_apply_forces(Particle *particle, vec3 force_sum, float delta);

void particle_integrate(Particle* particle, float damping, float delta);

#endif //PARTICLES_H
