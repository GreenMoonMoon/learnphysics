//
// Created by josue on 2024-02-26.
//

#ifndef GIZMOS_H
#define GIZMOS_H

#include "raylib.h"
#include "primitives.h"

typedef struct AddRectangleGizmo {
    Vector2 start;
    Vector2 end;
}AddRectangleGizmo;

typedef struct AddCubeGizmo {
    Vector2 start;
    Vector2 end;
    float height_start;
    float height_end;
} AddCubeGizmo;

void draw_aabb_on_plane_gizmo(const Plane plane, const AABB aabb, const Color color);

#endif //GIZMOS_H
