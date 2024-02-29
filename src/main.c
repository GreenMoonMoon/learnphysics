#include "raylib.h"
#include "raylib_utils.h"
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#include "flecs.h"

#include "collision.h"

#include "tools.h"
#define STB_DS_IMPLEMENTATION
#include "utils/stb_ds.h"
#include "scene.h"

static Camera3D camera;

static ecs_world_t *world;

static bool paused;

// static Scene3D basic_scene;
static Model *models = NULL;
static Light *lights = NULL;
static AABB *aabbs = NULL;

const Plane ground_plane = {.normal = {0.0f, 1.0f, 0.0f}, .distance = 0.0f};

static RayPrimitive mouse_ray = { 0 };     // Picking line ray
static Collision mouse_collision = { 0 };  // Ray collision hit info
static bool mouse_hit = false;

void init(void) {
    // Initialize window and render layer
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1080, 720, "Learn Physics");
    SetTargetFPS(60);
    set_line_width(2.0f);

    // Initialise Flecs
    // TODO: move to a scene setup
    world = ecs_init();

    // Initialize raylib components
    ECS_TAG(world, MainCamera);

    // Create camera
    camera = (Camera3D){
            .position = {0.0f, 3.0f, -8.0f},
            .up = {0.0f, 1.0f, 0.0f},
            .target = {0},
            .fovy = 70,
            .projection = CAMERA_PERSPECTIVE
    };

    // TODO: move to a game state struct
    paused = true;

    // SCENE

    // Initialize models
    Mesh cube_mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
    Material cube_material = LoadMaterialDefault();
    Shader cube_shader = LoadShader("../assets/shaders/default.vert", "../assets/shaders/default.frag");

    cube_shader.locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(cube_shader, "mvp");
    cube_shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(cube_shader, "viewPos");
    cube_shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(cube_shader, "instanceTransform");

    // Get some required shader locations for lighting
    cube_shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(cube_shader, "viewPos");

    // Ambient light level (some basic lighting)
    int ambient_loc = GetShaderLocation(cube_shader, "ambient");
    SetShaderValue(cube_shader, ambient_loc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);

    cube_material.shader = cube_shader;
    Model cube = LoadModelFromMesh(cube_mesh);
    cube.materials[0] = cube_material;
    arrput(models, cube);
    AABB aabb = {.min={-0.5f, -0.5f, -0.5f},.max={0.5f, 0.5f, 0.5f}};
    arrput(aabbs, aabb);

    // Initialize Lights
    lights = calloc(1, sizeof(Light));
    lights[0] = CreateLight(LIGHT_POINT, (Vector3){ -2, 1, -2 }, Vector3Zero(), WHITE, cube_shader);
}

void processInputs(void) {
    // View
    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) || GetMouseWheelMove() != 0.0f) {
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
    }

    // General
    if (IsKeyPressed(KEY_SPACE)) paused = !paused;

    // Select objects
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Ray in_ray = GetMouseRay(GetMousePosition(), camera);
        mouse_ray = (RayPrimitive){.origin = {in_ray.position.x, in_ray.position.y, in_ray.position.z},
                                   .direction = {in_ray.direction.x, in_ray.direction.y, in_ray.direction.z}};
        for (int i = 0; i < arrlen(aabbs); ++i) {
            if (get_collision_ray_aabb(mouse_ray, aabbs[i], &mouse_collision)) {
                mouse_hit = true;
                break;
            }
        }
    }
}

/// \brief Application loop
void run(void) {
    // Application loop
    while (!WindowShouldClose()) {
        processInputs();

        // Draw
        BeginDrawing();
        ClearBackground(DARKGRAY);

        BeginMode3D(camera);
        draw_grid();

        // Draw models
        for (int i = 0; i < arrlen(models); ++i) {
            DrawModel(models[i], Vector3Zero(), 1.0f, GRAY);
        }

        if(mouse_hit) {
            // Draw bounding box
            // Add overlay to the selected object
        }

        addRectangleToolDraw();

        // Draw scenes

        EndMode3D();

        DrawFPS(20, 20);
        if (paused) DrawText("PAUSED", 20, 50, 20, BLUE);

        EndDrawing();
    }
}

/// \brief Cleanup application and render layer just before exiting
void cleanup(void) {
    // Free lights
    free(lights);
    // Free models
    arrfree(models);

    ecs_fini(world);
    CloseWindow();
}

int main() {
    init();
    run();
    cleanup();

    return 0;
}
