//
// Created by josue on 2024-02-26.
//

#include "gizmos.h"
#include "rlgl.h"

void add_rectangle_gizmo_draw(const AddRectangleGizmo gizmo, const Color color) {
    rlPushMatrix();
    rlTranslatef(gizmo.start.x, 0.0f, gizmo.start.y);

    rlBegin(RL_LINES);
    rlColor4ub(color.r, color.g, color.b, color.a);

    // Front face
    //------------------------------------------------------------------
    // Bottom line
    rlVertex3f(0.0f, 0.0f, gizmo.end.y - gizmo.start.y);  // Bottom left
    rlVertex3f(gizmo.end.x - gizmo.start.x, 0.0f, gizmo.end.y - gizmo.start.y);  // Bottom right

    // Left line
    rlVertex3f(gizmo.end.x - gizmo.start.x, 0.0f, gizmo.end.y - gizmo.start.y);  // Bottom right
    rlVertex3f(gizmo.end.x - gizmo.start.x, 0.0f, gizmo.end.y - gizmo.start.y);  // Top right

    // Top line
    rlVertex3f(gizmo.end.x - gizmo.start.x, 0.0f, gizmo.end.y - gizmo.start.y);  // Top right
    rlVertex3f(0.0f, 0.0f, gizmo.end.y - gizmo.start.y);  // Top left

    // Right line
    rlVertex3f(0.0f, 0.0f, gizmo.end.y - gizmo.start.y);  // Top left
    rlVertex3f(0.0f, 0.0f, gizmo.end.y - gizmo.start.y);  // Bottom left

    // Back face
    //------------------------------------------------------------------
    // Bottom line
    rlVertex3f(0.0f, 0.0f, 0.0f);  // Bottom left
    rlVertex3f(gizmo.end.x - gizmo.start.x, 0.0f, 0.0f);  // Bottom right

    // Left line
    rlVertex3f(gizmo.end.x - gizmo.start.x, 0.0f, 0.0f);  // Bottom right
    rlVertex3f(gizmo.end.x - gizmo.start.x, 0.0f, 0.0f);  // Top right

    // Top line
    rlVertex3f(gizmo.end.x - gizmo.start.x, 0.0f, 0.0f);  // Top right
    rlVertex3f(0.0f, 0.0f, 0.0f);  // Top left

    // Right line
    rlVertex3f(0.0f, 0.0f, 0.0f);  // Top left
    rlVertex3f(0.0f, 0.0f, 0.0f);  // Bottom left

    // Top face
    //------------------------------------------------------------------
    // Left line
    rlVertex3f(0.0f, 0.0f, gizmo.end.y - gizmo.start.y);  // Top left front
    rlVertex3f(0.0f, 0.0f, 0.0f);  // Top left back

    // Right line
    rlVertex3f(gizmo.end.x - gizmo.start.x, 0.0f, gizmo.end.y - gizmo.start.y);  // Top right front
    rlVertex3f(gizmo.end.x - gizmo.start.x, 0.0f, 0.0f);  // Top right back

    // Bottom face
    //------------------------------------------------------------------
    // Left line
    rlVertex3f(0.0f, 0.0f, gizmo.end.y - gizmo.start.y);  // Top left front
    rlVertex3f(0.0f, 0.0f, 0.0f);  // Top left back

    // Right line
    rlVertex3f(gizmo.end.x - gizmo.start.x, 0.0f, gizmo.end.y - gizmo.start.y);  // Top right front
    rlVertex3f(gizmo.end.x - gizmo.start.x, 0.0f, 0.0f);  // Top right back
    rlEnd();
    rlPopMatrix();
}
