//
// Created by josue on 2024-03-04.
//

#include "draw_primitive.h"
#include "cglm/cglm.h"
#include "rlgl.h"

void plane_draw(Plane plane, Color color) {
    rlPushMatrix();
        // Create rotation matrix from normal
        // Create translation matrix from normal and distance

        // Draw plane
        rlBegin(RL_LINES);
            // Draw plane
            rlColor4ub(color.r, color.g, color.b, color.a);
            rlVertex3f(-2.0f, 0.0f, -2.0f);
            rlVertex3f(-2.0f, 0.0f, 2.0f);

            rlVertex3f(-2.0f, 0.0f, 2.0f);
            rlVertex3f(2.0f, 0.0f, 2.0f);

            rlVertex3f(2.0f, 0.0f, 2.0f);
            rlVertex3f(2.0f, 0.0f, -2.0f);

            rlVertex3f(2.0f, 0.0f, -2.0f);
            rlVertex3f(-2.0f, 0.0f, -2.0f);

            rlVertex3f(-2.5f, 0.0f, -2.5f);
            rlVertex3f(-2.5f, 0.0f, 2.5f);

            rlVertex3f(-2.5f, 0.0f, 2.5f);
            rlVertex3f(2.5f, 0.0f, 2.5f);

            rlVertex3f(2.5f, 0.0f, 2.5f);
            rlVertex3f(2.5f, 0.0f, -2.5f);

            rlVertex3f(2.5f, 0.0f, -2.5f);
            rlVertex3f(-2.5f, 0.0f, -2.5f);

            // Draw normal
            rlVertex3f(0.0f, 0.0f, 0.0f);
            rlVertex3f(0.0f, 2.0f, 0.0f);
        rlEnd();
    rlPopMatrix();
}

void aabb_draw(AABB aabb, Color color, float offset) {
    float width = aabb.max[0] - aabb.min[0] + offset * 2.0f;
    float height = aabb.max[1] - aabb.min[1] + offset * 2.0f;
    float depth = aabb.max[2] - aabb.min[2] + offset * 2.0f;

    rlPushMatrix();
    rlTranslatef(aabb.min[0] - offset, aabb.min[1] - offset, aabb.min[2] - offset);

    rlBegin(RL_LINES);
    rlColor4ub(color.r, color.g, color.b, color.a);

        // Front face
        //------------------------------------------------------------------
        // Bottom line
        rlVertex3f(0.0f, 0.0f, depth);  // Bottom left
        rlVertex3f(width, 0.0f, depth);  // Bottom right

        // Left line
        rlVertex3f(width, 0.0f, depth);  // Bottom right
        rlVertex3f(width, height, depth);  // Top right

        // Top line
        rlVertex3f(width, height, depth);  // Top right
        rlVertex3f(0.0f, height, depth);  // Top left

        // Right line
        rlVertex3f(0.0f, height, depth);  // Top left
        rlVertex3f(0.0f, 0.0f, depth);  // Bottom left

        // Back face
        //------------------------------------------------------------------
        // Bottom line
        rlVertex3f(0.0f, 0.0f, 0.0f);  // Bottom left
        rlVertex3f(width, 0.0f, 0.0f);  // Bottom right

        // Left line
        rlVertex3f(width, 0.0f, 0.0f);  // Bottom right
        rlVertex3f(width, height, 0.0f);  // Top right

        // Top line
        rlVertex3f(width, height, 0.0f);  // Top right
        rlVertex3f(0.0f, height, 0.0f);  // Top left

        // Right line
        rlVertex3f(0.0f, height, 0.0f);  // Top left
        rlVertex3f(0.0f, 0.0f, 0.0f);  // Bottom left

        // Top face
        //------------------------------------------------------------------
        // Left line
        rlVertex3f(0.0f, height, depth);  // Top left front
        rlVertex3f(0.0f, height, 0.0f);  // Top left back

        // Right line
        rlVertex3f(width, height, depth);  // Top right front
        rlVertex3f(width, height, 0.0f);  // Top right back

        // Bottom face
        //------------------------------------------------------------------
        // Left line
        rlVertex3f(0.0f, 0.0f, depth);  // Top left front
        rlVertex3f(0.0f, 0.0f, 0.0f);  // Top left back

        // Right line
        rlVertex3f(width, 0.0f, depth);  // Top right front
        rlVertex3f(width, 0.0f, 0.0f);  // Top right back
    rlEnd();
    rlPopMatrix();
}
