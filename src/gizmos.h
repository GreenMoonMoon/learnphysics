//
// Created by josue on 2024-02-26.
//

#ifndef GIZMOS_H
#define GIZMOS_H

#include "raylib.h"

typedef struct AddRectangleGizmo {
    Vector2 start;
    Vector2 end;
}AddRectangleGizmo;

void add_rectangle_gizmo_draw(AddRectangleGizmo gizmo, Color color);

#endif //GIZMOS_H
