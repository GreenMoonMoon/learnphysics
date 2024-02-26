#include "raylib.h"
#include "raylib_utils.h"
#include "particles.h"
#include "cglm/cglm.h"

#define FRAND(A) (float)rand()/(float)(RAND_MAX/A)

static Camera3D camera;
static bool paused;

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
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) || GetMouseWheelMove() != 0.0f) {
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
    }
    if(IsKeyPressed(KEY_SPACE)) paused = !paused;
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
