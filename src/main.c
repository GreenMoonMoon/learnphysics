#include <stdio.h>
#include "raylib.h"
#include "raylib_utils.h"
#include "rigidbody.h"
#include "collision.h"
#include "raymath.h"
#include "utils.h"

static Mesh mesh;
static Material material;
static Rigidbody rigidbodies[2];
static SphereCollider colliders[2];

static bool paused;

int main() {
    InitWindow(1080, 720, "Learn Physics");
    SetTargetFPS(60);

    // Initialize camera
    Camera3D camera = {
            .position = {0.0f, 3.0f, -8.0f},
            .up = {0.0f, 1.0f, 0.0f},
            .target = {0},
            .fovy = 70,
            .projection = CAMERA_PERSPECTIVE
    };

    // Load entities
    mesh = GenMeshSphere(0.5f, 8, 8);
    material = LoadMaterialDefault();
    material.maps[MATERIAL_MAP_DIFFUSE].color = BLUE;

    rigidbodies[0] = (Rigidbody){
        .velocity = {0.0f, -1.0f, 0.0f},
        .transform = GLM_MAT4_IDENTITY_INIT
    };
    colliders[0] = (SphereCollider){{0}, .radius = 0.5f};
    vec3 position_0 = {0.0f, 2.0f, 0.0f};
    glm_translate(rigidbodies[0].transform, position_0);

    rigidbodies[1] = (Rigidbody){
        .velocity = {0.0f, -2.0f, 1.0f},
        .transform = GLM_MAT4_IDENTITY_INIT
    };
    colliders[1] = (SphereCollider){{0}, .radius = 0.5f};
    vec3 position_1 = {0.5f, 4.0f, 0.5f};
    glm_translate(rigidbodies[1].transform, position_1);

    // xyz is the normal, w is the offset along the normal
    vec4 ground_plane = {0.0f, 1.0f, 0.0f, 0.0f};

    paused = true;

    // Application loop
    while(!WindowShouldClose()) {
        // Update
        float delta_time = GetFrameTime();

        // Inputs
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        }
        if(IsKeyPressed(KEY_SPACE)) paused = !paused;

        // Physic update
        if (!paused) {
            for(int i = 0; i < 2; i++) {
                vec3 forces = GLM_VEC3_ZERO_INIT;

                // Update forces
                glm_vec3_add(forces, (vec3){0.0f, -9.8f, 0.0f}, forces);

                // Integrate velocity
                integrate_linear(&rigidbodies[i], forces, delta_time);
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(DARKGRAY);

        BeginMode3D(camera);
        DrawGrid(16, 1);
        DrawLine3D(Vector3Zero(), (Vector3){5.0f, 0.0f, 0.0f}, RED);
        DrawLine3D(Vector3Zero(), (Vector3){0.0f, 5.0f, 0.0f}, GREEN);
        DrawLine3D(Vector3Zero(), (Vector3){0.0f, 0.0f, 5.0f}, BLUE);

        for(int i = 0; i < 2; i++) {
            const auto mat = MAT4_TO_MATRIX(rigidbodies[i].transform);
            DrawMesh(mesh, material, mat);
        }

        EndMode3D();

        DrawFPS(20, 20);
        if (paused) DrawText("PAUSED", 20, 50, 20, BLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
