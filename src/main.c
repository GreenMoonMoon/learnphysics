#include <stdio.h>
#include "raylib.h"
#include "raylib_utils.h"
#include "rigidbody.h"

static Mesh mesh;
static Material material;
static Rigidbody rigidbodies[2];

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

    // Load entities
    mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
    material = LoadMaterialDefault();
    material.maps[MATERIAL_MAP_DIFFUSE].color = BLUE;

    rigidbodies[0] = (Rigidbody){
        .velocity = {0.0f, -1.0f, 0.0f},
        .transform = GLM_MAT4_IDENTITY_INIT
    };
    vec3 position_0 = {0.0f, 2.0f, 0.0f};
    glm_translate(rigidbodies[0].transform, position_0);

    rigidbodies[1] = (Rigidbody){
        .velocity = {0.0f, -2.0f, 1.0f},
        .transform = GLM_MAT4_IDENTITY_INIT
    };
    vec3 position_1 = {0.5f, 4.0f, 0.5f};
    glm_translate(rigidbodies[1].transform, position_1);

    // Application loop
    while(!WindowShouldClose()) {
        // Update
        float delta_time = GetFrameTime();

        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        }
        for(int i = 0; i < 2; i++) {
            integrate_velocity(&rigidbodies[i], delta_time);
        }

        // Draw
        BeginDrawing();
        ClearBackground(DARKGRAY);

        BeginMode3D(camera);
        DrawGrid(16, 1);

        for(int i = 0; i < 2; i++) {
            const auto mat = MAT4_TO_MATRIX(rigidbodies[i].transform);
            DrawMesh(mesh, material, mat);
        }

        EndMode3D();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
