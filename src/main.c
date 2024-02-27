#include "raylib.h"
#include "raylib_utils.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#include "collision.h"
#define STB_DS_IMPLEMENTATION
#include "utils/stb_ds.h"

#include "tools.h"

#define FRAND(A) (float)rand()/(float)(RAND_MAX/A)

static Camera3D camera;
static bool paused;

const Plane ground_plane = {.normal = {0.0f, 1.0f, 0.0f}, .distance = 0.0f};

static Matrix *cubes = NULL;
static Mesh cube_mesh;
static Material cube_material;

void instantiateCube(vec3 position, vec3 size) {
    Matrix transform = MatrixIdentity();
    transform = MatrixMultiply(transform, MatrixTranslate(position[0], position[1], position[2]));
    transform = MatrixMultiply(transform, MatrixScale(size[0], size[1], size[2]));

            arrput(cubes, transform);
}

/// \brief Initialize application and render layer
void init(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1080, 720, "Learn Physics");
    SetTargetFPS(60);
    set_line_width(2.0f);

    // Initialize camera
    camera = (Camera3D) {
            .position = {0.0f, 3.0f, -8.0f},
            .up = {0.0f, 1.0f, 0.0f},
            .target = {0},
            .fovy = 70,
            .projection = CAMERA_PERSPECTIVE
    };

    paused = true;

    cube_mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
    cube_material = LoadMaterialDefault();
    Shader cube_shader = LoadShader("../assets/shaders/default_instanced.vert", "../assets/shaders/default.frag");

    cube_shader.locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(cube_shader, "mvp");
    cube_shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(cube_shader, "viewPos");
    cube_shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(cube_shader, "instanceTransform");

    // Get some required shader locations for lighting
    cube_shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(cube_shader, "viewPos");
    // NOTE: "matModel" location name is automatically assigned on shader loading,
    // no need to get the location again if using that uniform name
    //shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");

    // Ambient light level (some basic lighting)
    int ambientLoc = GetShaderLocation(cube_shader, "ambient");
    SetShaderValue(cube_shader, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);

    cube_material.shader = cube_shader;

    // Initialise Lights
    Light lights[MAX_LIGHTS] = { 0 };
    lights[0] = CreateLight(LIGHT_POINT, (Vector3){ -2, 1, -2 }, Vector3Zero(), WHITE, cube_shader);
}

/// \brief Process application inputs
void processInputs(void) {
    // View
    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) || GetMouseWheelMove() != 0.0f) {
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
    }

    // General
    if (IsKeyPressed(KEY_SPACE)) paused = !paused;

    // Get mouse ray for future calculations
    Ray in_ray = GetMouseRay(GetMousePosition(), camera);
    RayPrimitive mouse_ray = {.origin = {in_ray.position.x, in_ray.position.y, in_ray.position.z},
            .direction = {in_ray.direction.x, in_ray.direction.y, in_ray.direction.z}};

    // TOOLS
    // Add Rectangle
    addRectangleUpdate(mouse_ray, ground_plane, camera);

}

/// \brief Application loop
void run(void) {
    // Application loop
    while (!WindowShouldClose()) {
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

        addRectangleDraw();

        // Draw cube primitives
        if(cubes != NULL){ DrawMeshInstanced(cube_mesh, cube_material, cubes, arrlen(cubes)); }

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
