#include <stdio.h>
#include "raylib.h"
#include "raylib_utils.h"
#include "rigidbody.h"
#include "collision.h"

static Mesh mesh;
static Material material;
static Rigidbody rigidbodies[2];
static SphereCollider colliders[2];

static bool paused;

void reflect(vec3 vector, vec3 normal) {
    // I - 2.0 * dot(N, I) * N
    // where I is a vector and N the surface normal
    vec3 n;
    glm_vec3_scale(normal, 2.0f * glm_dot(normal, vector), n);
    glm_vec3_sub(vector, n, vector);
}

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
    mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
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
    vec4 wall_a = { 1.0f,  0.0f,  0.0f,  8.0f};
    vec4 wall_b = {-1.0f,  0.0f,  0.0f, -8.0f};
    vec4 wall_c = { 0.0f,  0.0f,  1.0f,  8.0f};
    vec4 wall_d = { 0.0f,  0.0f, -1.0f, -8.0f};

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
                // Update forces
                vec3 forces;
                glm_vec2_add(forces, (float[3]){0.0f, -9.8f, 0.0f}, forces);

                // Check collisions
                Collision collision;
                if(get_sphere_plane_collision(colliders[i], rigidbodies[i].transform[3], ground_plane, &collision)) {
                    // resolve penetration and reflect velocity
                    vec3 penetration;
                    glm_vec3_scale(ground_plane, collision.depth, penetration);
                    glm_translate(rigidbodies[i].transform, penetration);
                    reflect(rigidbodies[i].velocity, ground_plane);
                }
                if(get_sphere_plane_collision(colliders[i], rigidbodies[i].transform[3], wall_a, &collision)) {
                    // resolve penetration and reflect velocity
                    vec3 penetration;
                    glm_vec3_scale(wall_a, collision.depth, penetration);
                    glm_translate(rigidbodies[i].transform, penetration);
                    reflect(rigidbodies[i].velocity, wall_a);
                }
                if(get_sphere_plane_collision(colliders[i], rigidbodies[i].transform[3], wall_b, &collision)) {
                    // resolve penetration and reflect velocity
                    vec3 penetration;
                    glm_vec3_scale(wall_b, collision.depth, penetration);
                    glm_translate(rigidbodies[i].transform, penetration);
                    reflect(rigidbodies[i].velocity, wall_b);
                }
                if(get_sphere_plane_collision(colliders[i], rigidbodies[i].transform[3], wall_c, &collision)) {
                    // resolve penetration and reflect velocity
                    vec3 penetration;
                    glm_vec3_scale(wall_c, collision.depth, penetration);
                    glm_translate(rigidbodies[i].transform, penetration);
                    reflect(rigidbodies[i].velocity, wall_c);
                }
                if(get_sphere_plane_collision(colliders[i], rigidbodies[i].transform[3], wall_d, &collision)) {
                    // resolve penetration and reflect velocity
                    vec3 penetration;
                    glm_vec3_scale(wall_d, collision.depth, penetration);
                    glm_translate(rigidbodies[i].transform, penetration);
                    reflect(rigidbodies[i].velocity, wall_d);
                }

                // Integrate velocity
                integrate_linear(&rigidbodies[i], forces, delta_time);
            }
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

        DrawFPS(20, 20);
        if (paused) DrawText("PAUSED", 20, 50, 20, BLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
