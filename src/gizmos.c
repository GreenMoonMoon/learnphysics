//
// Created by josue on 2024-02-26.
//

#include "gizmos.h"

void add_rectangle_gizmo_draw(const AddRectangleGizmo gizmo, const Color color) {
    DrawCubeWires((Vector3){gizmo.start.x, 0.0f, gizmo.start.y},
                  gizmo.end.x - gizmo.start.x,
                  0.0f,
                  gizmo.end.y - gizmo.start.y,
                  color);
}
