#include <stdio.h>
#include "raylib.h"
#include "raymath.h"

#include "rigidbody.h"

typedef struct Entity {
    Mesh mesh;
    Material material;
    Matrix transform;
} Entity;
Entity entities[2];

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
    entities[0] = (Entity){
        .mesh = GenMeshCube(1.0f, 1.0f, 1.0f),
        .material = LoadMaterialDefault(),
        .transform = MatrixTranslate(0.0f, 2.0f, 0.0f)
    };
    entities[0].material.maps[MATERIAL_MAP_DIFFUSE].color = BLUE;

    entities[1] = (Entity){
            .mesh = GenMeshCube(1.0f, 1.0f, 1.0f),
            .material = LoadMaterialDefault(),
            .transform = MatrixTranslate(0.5f, 4.0f, 0.5f)
    };
    entities[1].material.maps[MATERIAL_MAP_DIFFUSE].color = RED;

    while(!WindowShouldClose()) {
        // Update
//        UpdateCamera(&camera, CAMERA_FREE);

        // Draw
        BeginDrawing();
        ClearBackground(DARKGRAY);

        BeginMode3D(camera);
        DrawGrid(16, 1);

        for(int i = 0; i < 2; i++) {
            DrawMesh(entities[i].mesh, entities[i].material, entities[i].transform);
        }

        EndMode3D();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
