//
// Created by josue on 2024-02-26.
//

#include "tools.h"
#include "collision.h"
#include "gizmos.h"

static struct {
    AABB rectangle;
    Plane plane;
    vec3 intersection_point;
    uint8_t step; // step == 0 is inactive
} add_rectangle_tool = {0};

void addRectangleToolInit(const Plane base_plane) {
    add_rectangle_tool.plane = base_plane;
}

bool addRectangleTool(const RayPrimitive mouse_ray, const Camera3D camera, AddRectangleParam *param) {
    if (!add_rectangle_tool.step) { // Activate tool
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            get_ray_plane_intersection(mouse_ray, add_rectangle_tool.plane, add_rectangle_tool.intersection_point)) {
            add_rectangle_tool.step++; // Go to the initial "active" step.
            // Initial values
            glm_vec3_copy(add_rectangle_tool.intersection_point,
                          add_rectangle_tool.rectangle.min);
            glm_vec3_copy(add_rectangle_tool.intersection_point,
                          add_rectangle_tool.rectangle.max);
        }
    } else {
        const bool intersects = get_ray_plane_intersection(mouse_ray,
                                                           add_rectangle_tool.plane,
                                                           add_rectangle_tool.intersection_point);
        const Matrix camera_matrix = GetCameraMatrix(camera);
        vec3 x_axis = {1.0f, 0.0f, 0.0f};
        vec3 view_direction = {camera_matrix.m9, camera_matrix.m10, camera_matrix.m11};
        const float view_x_axis = fabsf(glm_vec3_dot(view_direction, x_axis));

        switch (add_rectangle_tool.step) {
            case 1:
                if (intersects) {
                    add_rectangle_tool.rectangle.max[0] = add_rectangle_tool.intersection_point[0];
                    add_rectangle_tool.rectangle.max[2] = add_rectangle_tool.intersection_point[2];
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) { // Mouse on to the next step
                    add_rectangle_tool.step++;
                }
                break;
            case 2: // last step
            default:
                // Get plane according to view direction
                if(view_x_axis < 0.5f ) {
                    add_rectangle_tool.plane = (Plane) {{1.0f, 0.0f, 0.0f}, add_rectangle_tool.rectangle.max[0]};
                } else {
                    add_rectangle_tool.plane = (Plane) {{0.0f, 0.0f, 1.0f}, add_rectangle_tool.rectangle.max[2]};
                }
                if (intersects) {
                    add_rectangle_tool.rectangle.max[1] = add_rectangle_tool.intersection_point[1];
                }
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    add_rectangle_tool.step = 0; // clear tool
                    vec3 size;
                    glm_vec3_sub(add_rectangle_tool.rectangle.max, add_rectangle_tool.rectangle.min, size);
                    glm_vec3_copy(add_rectangle_tool.rectangle.min, param->position);
                    glm_vec3_sub(add_rectangle_tool.rectangle.max, add_rectangle_tool.rectangle.min, param->size);
                    return true;
                }
                break;
        }

        // Canceling operation
        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
            add_rectangle_tool.step = 0;
        }

    }
    return false;
}

void addRectangleToolDraw() {
    // Draw "Add Rectangle" gizmo
    if (add_rectangle_tool.step) {
        draw_aabb_on_plane_gizmo(add_rectangle_tool.plane, add_rectangle_tool.rectangle, GREEN);
    }
}
