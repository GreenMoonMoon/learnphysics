#include <stdio.h>
#include "raylib.h"

#include "rigidbody.h"


int main() {
    InitWindow(1080, 720, "Learn Physics");

    // Initialize camera
    Camera3D camera = {
            .position = {0.0f, 3.0f, -8.0f},
            .up = {0.0f, 1.0f, 0.0f},
            .target = {0},
            .fovy = 70,
            .projection = CAMERA_PERSPECTIVE
    };

    while(!WindowShouldClose()) {
        // Update
//        UpdateCamera(&camera, CAMERA_FREE);

        // Draw
        BeginDrawing();
        ClearBackground(DARKGRAY);

        BeginMode3D(camera);
        DrawGrid(16, 1);
        EndMode3D();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
