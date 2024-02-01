#include <stdio.h>
#include "raylib.h"
#include "raylib_utils.h"
#include "particles.h"
#include "raymath.h"
#include "cglm/cglm.h"
#include "cglm/vec3-ext.h"

#define FRAND(A) (float)rand()/(float)(RAND_MAX/A)

static bool paused;

static void vec3_reflect(vec3 vector, vec3 normal, vec3 dest) {
    // I - 2.0 * dot(N, I) * N.
    // Based on glsl documentation.
    glm_vec3_scale(normal, 2.0f * glm_vec3_dot(normal, vector), normal);
    glm_vec3_sub(vector, normal, dest);
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

    paused = true;

    Particle particles[25];
    for(int i = 0; i < 25; i++) {
        particles[i] = (Particle){
            .position = {0.0f, 0.1f, 0.0f},
            .velocity = {FRAND(5.0f) - 2.5f, 10.0f, FRAND(5.0f) - 2.5f},
            .force_sum =  {0},
            .inverse_mass =  1.0f
        };
    }

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
            for (int i = 0; i < 25; i++) {
                particle_apply_forces(&particles[i], (vec3){0.0f, -10.0f, 0.0f});
                particle_integrate(&particles[i], 0.9f, GetFrameTime());

                // Simple collisisons
                if(particles[i].position[1] <= 0.0f) {
                    particles[i].position[1] = 0.0f;
                    vec3_reflect(particles[i].velocity, (vec3){0.0f, 1.0f, 0.0f}, particles[i].velocity);
                    glm_vec3_scale(particles[i].velocity, 0.75f, particles[i].velocity); // Apply restitution
                }
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(DARKGRAY);

        BeginMode3D(camera);
        draw_grid();

        for (int i = 0; i < 25; i++) { draw_particle(particles[i]); }

        EndMode3D();

        DrawFPS(20, 20);
        if (paused) DrawText("PAUSED", 20, 50, 20, BLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
