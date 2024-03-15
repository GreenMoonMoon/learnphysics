//
// Created by josue on 2024-03-04.
//

#ifndef LEARNPHYSICS_DRAW_PRIMITIVE_H
#define LEARNPHYSICS_DRAW_PRIMITIVE_H

#include "primitives.h"
#include "raylib.h"

void plane_draw(Plane plane, Color color);

void aabb_draw(AABB aabb, Color color, float offset);

#endif //LEARNPHYSICS_DRAW_PRIMITIVE_H
