//
// Created by josue on 2024-02-26.
//

#include "gizmos.h"
#include "rlgl.h"

void add_rectangle_gizmo_draw(const AddRectangleGizmo gizmo, const Color color) {
    float x = gizmo.end.x - gizmo.start.x;
    float z = gizmo.end.y - gizmo.start.y;

    rlPushMatrix();
    rlTranslatef(gizmo.start.x, 0.0f, gizmo.start.y);

    rlBegin(RL_LINES);
    rlColor4ub(color.r, color.g, color.b, color.a);

    // Front line
    rlVertex3f(0.0f, 0.0f, z);  // Bottom left
    rlVertex3f(x, 0.0f, z);  // Bottom right

    // Back line
    rlVertex3f(0.0f, 0.0f, 0.0f);  // Bottom left
    rlVertex3f(x, 0.0f, 0.0f);  // Bottom right

    // Left line
    rlVertex3f(0.0f, 0.0f, z);  // Top left front
    rlVertex3f(0.0f, 0.0f, 0.0f);  // Top left back

    // Right line
    rlVertex3f(x, 0.0f, z);  // Top right front
    rlVertex3f(x, 0.0f, 0.0f);  // Top right back

    rlEnd();
    rlPopMatrix();
}

void add_cube_gizmo_draw(const AddCubeGizmo gizmo, const Color color) {
    float x = gizmo.end.x - gizmo.start.x;
    float y = gizmo.height_end - gizmo.height_start;
    float z = gizmo.end.y - gizmo.start.y;

    rlPushMatrix();
    rlTranslatef(gizmo.start.x, 0.0f, gizmo.start.y);

    rlBegin(RL_LINES);
    rlColor4ub(color.r, color.g, color.b, color.a);

    // Front face
    //------------------------------------------------------------------
    // Bottom line
    rlVertex3f(0.0f, 0.0f, z);  // Bottom left
    rlVertex3f(x, 0.0f, z);  // Bottom right

    // Left line
    rlVertex3f(x, 0.0f, z);  // Bottom right
    rlVertex3f(x, y, z);  // Top right

    // Top line
    rlVertex3f(x, y, z);  // Top right
    rlVertex3f(0.0f, y, z);  // Top left

    // Right line
    rlVertex3f(0.0f, y, z);  // Top left
    rlVertex3f(0.0f, 0.0f, z);  // Bottom left

    // Back face
    //------------------------------------------------------------------
    // Bottom line
    rlVertex3f(0.0f, 0.0f, 0.0f);  // Bottom left
    rlVertex3f(x, 0.0f, 0.0f);  // Bottom right

    // Left line
    rlVertex3f(x, 0.0f, 0.0f);  // Bottom right
    rlVertex3f(x, y, 0.0f);  // Top right

    // Top line
    rlVertex3f(x, y, 0.0f);  // Top right
    rlVertex3f(0.0f, y, 0.0f);  // Top left

    // Right line
    rlVertex3f(0.0f, y, 0.0f);  // Top left
    rlVertex3f(0.0f, 0.0f, 0.0f);  // Bottom left

    // Top face
    //------------------------------------------------------------------
    // Left line
    rlVertex3f(0.0f, y, z);  // Top left front
    rlVertex3f(0.0f, y, 0.0f);  // Top left back

    // Right line
    rlVertex3f(x, y, z);  // Top right front
    rlVertex3f(x, y, 0.0f);  // Top right back

    // Bottom face
    //------------------------------------------------------------------
    // Left line
    rlVertex3f(0.0f, 0.0f, z);  // Top left front
    rlVertex3f(0.0f, 0.0f, 0.0f);  // Top left back

    // Right line
    rlVertex3f(x, 0.0f, z);  // Top right front
    rlVertex3f(x, 0.0f, 0.0f);  // Top right back

    rlEnd();
    rlPopMatrix();
}

void draw_aabb_on_gizmo(const PlanePrimitive plane, const AABB aabb, const Color color) {
    rlPushMatrix();
//    rlTranslatef(gizmo.start.x, 0.0f, gizmo.start.y);

    rlBegin(RL_LINES);
    rlColor4ub(color.r, color.g, color.b, color.a);
    rlEnd();
}
