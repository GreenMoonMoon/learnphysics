#include "raylib.h"
#include "raylib_utils.h"
#include "particles.h"
#include "cglm/cglm.h"

#define FRAND(A) (float)rand()/(float)(RAND_MAX/A)

static Camera3D camera;
static bool paused;

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

void process_inputs(void) {
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) || GetMouseWheelMove() != 0.0f) {
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
    }
    if(IsKeyPressed(KEY_SPACE)) paused = !paused;
}

void cleanup(void) {
    CloseWindow();
}

int main() {
    init();

    Particle particles[25];
    for(int i = 0; i < 25; i++) {
        particles[i] = (Particle){
            .position = {0.0f, 0.1f, 0.0f},
            .velocity = {FRAND(5.0f) - 2.5f, 10.0f, FRAND(5.0f) - 2.5f},
            .inverse_mass =  1.0f
        };
    }

    // Application loop
    while(!WindowShouldClose()) {
        // Update
        float delta_time = GetFrameTime();
        process_inputs();

        // Physic update
        if (!paused) {
            for (int i = 0; i < 25; i++) {
                // Add forces
                vec3 force_sum = {0};
                glm_vec3_add(force_sum, (vec3){0.0f, -10.0f, 0.0f}, force_sum); // Add gravity
                particle_apply_forces(&particles[i], force_sum, GetFrameTime());
                particle_integrate(&particles[i], 0.9f, GetFrameTime());
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

    cleanup();
    return 0;
}
