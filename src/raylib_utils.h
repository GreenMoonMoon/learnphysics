//
// Created by josue on 2024-01-24.
//

#ifndef RAYLIB_UTILS_H
#define RAYLIB_UTILS_H

#include "raylib.h"
#include "particles.h"

#define MAT4_TO_MATRIX(M) ((Matrix){ M[0][0],M[1][0],M[2][0],M[3][0],M[0][1],M[1][1],M[2][1],M[3][1],M[0][2],M[1][2],M[2][2],M[3][2],M[0][3],M[1][3],M[2][3],M[3][3]})

void set_line_width(float width);

void draw_grid(void);

void draw_particle(Particle particle);

#endif //RAYLIB_UTILS_H
