#include "raylib.h"
#include "raylib_utils.h"
#include "cglm/cglm.h"

#include "gizmos.h"
#include "collision.h"

#define FRAND(A) (float)rand()/(float)(RAND_MAX/A)

static Camera3D camera;
static bool paused;

// TEST GIZMOS
static AddRectangleGizmo add_rectangle_gizmo;
const PlanePrimitive xz_plane = {.normal = {0.0f, 1.0f, 0.0f}, .distance = 0.0f};

/// \brief Initialize application and render layer
void init(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1080, 720, "Learn Physics");
    SetTargetFPS(60);
    set_line_width(2.0f);

    // Initialize camera
    camera = (Camera3D){
            .position = {0.0f, 3.0f, -8.0f},
            .up = {0.0f, 1.0f, 0.0f},
            .target = {0},
            .fovy = 70,
            .projection = CAMERA_PERSPECTIVE
    };

    paused = true;
}

/// \brief Process application inputs
void process_inputs(void) {
    // View
    if(IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) || GetMouseWheelMove() != 0.0f) {
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
    }

    // General
    if(IsKeyPressed(KEY_SPACE)) paused = !paused;

    // Test mouse ray and default plane intersection
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Ray in_ray = GetMouseRay(GetMousePosition(), camera);
        RayPrimitive ray = {.origin = {in_ray.position.x, in_ray.position.y, in_ray.position.z},
                            .direction = {in_ray.direction.x, in_ray.direction.y, in_ray.direction.z}};
        vec3 intersection_point;
        if (get_ray_plane_intersection(ray, xz_plane, intersection_point)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                add_rectangle_gizmo = (AddRectangleGizmo) {0};
                add_rectangle_gizmo.start = (Vector2) {intersection_point[0], intersection_point[2]};
                add_rectangle_gizmo.end = add_rectangle_gizmo.start;
            }

            add_rectangle_gizmo.end = (Vector2) {intersection_point[0], intersection_point[2]};
        }
    }
}

/// \brief Application loop
void run(void) {
    // Application loop
    while(!WindowShouldClose()) {
        // Update
        float delta_time = GetFrameTime();
        process_inputs();

        // Physic update
        if (!paused) {
        }

        // Draw
        BeginDrawing();
        ClearBackground(DARKGRAY);

        BeginMode3D(camera);
        draw_grid();

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) { add_rectangle_gizmo_draw(add_rectangle_gizmo, GREEN); }

        EndMode3D();

        DrawFPS(20, 20);
        if (paused) DrawText("PAUSED", 20, 50, 20, BLUE);

        EndDrawing();
    }
}

/// \brief Cleanup application and render layer just before exiting
void cleanup(void) {
    CloseWindow();
}

int main() {
    // Simple app
    init();
    run();
    cleanup();
    return 0;
}
