#include "raylib.h"
#include "raylib_utils.h"
#include "utils/stb_ds.h"
#include "cglm/cglm.h"

#include "gizmos.h"
#include "collision.h"

#define FRAND(A) (float)rand()/(float)(RAND_MAX/A)

static Camera3D camera;
static bool paused;

static Matrix *cubes = NULL;

const Plane ground_plane = {.normal = {0.0f, 1.0f, 0.0f}, .distance = 0.0f};

// TEST GIZMOS
// "Add cube" tool
static struct {
    AABB rectangle;
    Plane plane;
    uint8_t step; // step == 0 is inactive
} add_cube_tool = {0};

void instanciateCube(Vector3 size, Vector3 position) {
    Matrix transform = MatrixIdentity();
    transform = MatrixMultiply(transform, MatrixScale(size.x, size.y, size.z));
    transform = MatrixMultiply(transform, MatrixTranslate(position.x, position.y, position.z));

    arrpush(cubes, transform);
}

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
void processInputs(void) {
    // View
    if(IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) || GetMouseWheelMove() != 0.0f) {
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
    }

    // General
    if(IsKeyPressed(KEY_SPACE)) paused = !paused;

    // Get mouse ray for future calculations
    Ray in_ray = GetMouseRay(GetMousePosition(), camera);
    RayPrimitive ray = {.origin = {in_ray.position.x, in_ray.position.y, in_ray.position.z},
            .direction = {in_ray.direction.x, in_ray.direction.y, in_ray.direction.z}};

    // TOOLS
    // Update "Add Cube" tool
    if(!add_cube_tool.step && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { // Activate tool
        add_cube_tool.step++; // Go to the initial "active" step.

        add_cube_tool.plane = ground_plane;
    }


    vec3 intersection_point;
    if(get_ray_plane_intersection(ray, xz_plane, intersection_point)) {

        // Update "Add Cube" tool.
        switch (add_cube_tool.step) {
            case 1:  // Base rect step
                add_cube_tool.gizmo.end = (Vector2){intersection_point[0], intersection_point[2]};
                if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    add_cube_tool.gizmo.height_start = GetMousePosition().y;
                    add_cube_tool.step = 2;
                }
                break;
            case 2:  // Height step
                add_cube_tool.gizmo.height_end = GetMousePosition().y;
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    instanciateCube();
                    add_cube_tool.step = 0;
                }
                break;
            default: // uninitialized
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { // Initialize tool
                    add_cube_tool.step = 1;
                    add_cube_tool.gizmo = (AddCubeGizmo) {
                            .start = (Vector2) {intersection_point[0], intersection_point[2]},
                            .end = (Vector2) {intersection_point[0], intersection_point[2]},
                            .height_start = 0.0f,
                            .height_end = 0.0f
                    };
                }
        }
    }
}

/// \brief Application loop
void run(void) {
    // Application loop constants
    // initialize cube mesh and material as constant
    const Mesh cube_mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
    const Material cube_material = LoadMaterialDefault();

    // Application loop
    while(!WindowShouldClose()) {
        // Update
        float delta_time = GetFrameTime();
        processInputs();

        // Physic update
        if (!paused) {
        }

        // Draw
        BeginDrawing();
        ClearBackground(DARKGRAY);

        BeginMode3D(camera);
        draw_grid();

        // Draw cube primitives
        DrawMeshInstanced(cube_mesh, cube_material, cubes, arrlen(cubes));

        EndMode3D();

        DrawFPS(20, 20);
        if (paused) DrawText("PAUSED", 20, 50, 20, BLUE);

        EndDrawing();
    }
}

/// \brief Cleanup application and render layer just before exiting
void cleanup(void) {
    arrfree(cubes);

    CloseWindow();
}

int main() {
    // Simple app
    init();
    run();
    cleanup();

    return 0;
}
