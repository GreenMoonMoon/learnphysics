#include "raylib.h"
#include "raylib_utils.h"
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#include "flecs.h"

#include "collision.h"

#include "tools.h"
#define STB_DS_IMPLEMENTATION
#include "utils/stb_ds.h"

#include "graphic/draw_primitive.h"

static Camera3D camera;

static ecs_world_t *world;

// Declare components
ECS_COMPONENT_DECLARE(Plane);
ECS_COMPONENT_DECLARE(AABB);

// Declare systems
ECS_SYSTEM_DECLARE(draw_planes);
ECS_SYSTEM_DECLARE(draw_aabbs);

static bool paused;

// static Scene3D basic_scene;
static Model *models = NULL;
static Light *lights = NULL;
static AABB *aabbs = NULL;

static RayPrimitive mouse_ray = { 0 };     // Picking line ray
static Collision mouse_collision = { 0 };  // Ray collision hit info
static bool mouse_hit = false;

void load_basic_scene(void) {
    // Load models
    // Add a ground
    ecs_entity_t ground = ecs_new_id(world);
    Plane ground_plane = {.normal = {0.0f, 1.0f, 0.0f}, .distance = 0.0f};
    ecs_set(world, ground, Plane, {});

    // Add a few cubes
    ecs_entity_t cube_a = ecs_new_id(world);
    ecs_set(world, cube_a, AABB, {.min = {0.0f, 0.0f, 0.0f}, .max = {1.0f, 1.0f, 1.0f}});
    ecs_entity_t cube_b = ecs_new_id(world);
    ecs_set(world, cube_b, AABB, {.min = {0.0f, 0.0f, 0.0f}, .max = {1.0f, 1.0f, 1.0f}});
    ecs_entity_t cube_c = ecs_new_id(world);
    ecs_set(world, cube_c, AABB, {.min = {0.0f, 0.0f, 0.0f}, .max = {1.0f, 1.0f, 1.0f}});
    ecs_entity_t cube_d = ecs_new_id(world);
    ecs_set(world, cube_d, AABB, {.min = {0.0f, 0.0f, 0.0f}, .max = {1.0f, 1.0f, 1.0f}});
    ecs_entity_t cube_e = ecs_new_id(world);
    ecs_set(world, cube_e, AABB, {.min = {0.0f, 0.0f, 0.0f}, .max = {1.0f, 1.0f, 1.0f}});

    // Load lights
    Light light_a = init_light(LIGHT_POINT, (Vector3){ -2, 1, -2 }, Vector3Zero(), WHITE);
}

void draw_planes(ecs_iter_t *it) {
    Plane *p = ecs_field(it, Plane, 1);

    for (int i = 0; i < it->count; ++i) {
        draw_plane(*p, GREEN);
    }
}

void draw_aabbs(ecs_iter_t *it) {
    AABB *a = ecs_field(it, AABB, 1);

    for (int i = 0; i < it->count; ++i) {
        draw_aabb(*a, BLUE);
    }
}

void init(void) {
    // Initialize window and render layer
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1080, 720, "Learn Physics");
    SetTargetFPS(60);
    set_line_width(2.0f);

    // Initialize Flecs
    world = ecs_init();

    // Initialize components
    ECS_COMPONENT_DEFINE(world, Plane);
    ECS_COMPONENT_DEFINE(world, AABB);

    // Initialize tags
    ECS_TAG(world, MainCamera);

    // Initialize systems
    ECS_SYSTEM_DEFINE(world, draw_planes, EcsOnStore, [in]Plane);
    ECS_SYSTEM_DEFINE(world, draw_aabbs, EcsOnStore, [in]AABB);

    // Create camera
    camera = (Camera3D){
            .position = {0.0f, 3.0f, -8.0f},
            .up = {0.0f, 1.0f, 0.0f},
            .target = {0},
            .fovy = 70,
            .projection = CAMERA_PERSPECTIVE
    };

    paused = true;

    // SCENE
    load_basic_scene();

//    // Initialize models
//    Mesh cube_mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
//    Material cube_material = LoadMaterialDefault();
//    Shader cube_shader = LoadShader("../assets/shaders/default.vert", "../assets/shaders/default.frag");
//
//    cube_shader.locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(cube_shader, "mvp");
//    cube_shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(cube_shader, "viewPos");
//    cube_shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(cube_shader, "instanceTransform");
//
//    // Get some required shader locations for lighting
//    cube_shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(cube_shader, "viewPos");
//
//    // Ambient light level (some basic lighting)
//    int ambient_loc = GetShaderLocation(cube_shader, "ambient");
//    SetShaderValue(cube_shader, ambient_loc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);
//
//    cube_material.shader = cube_shader;
//    Model cube = LoadModelFromMesh(cube_mesh);
//    cube.materials[0] = cube_material;
//    arrput(models, cube);
//    AABB aabb = {.min={-0.5f, -0.5f, - 0.5f},.max={0.5f, 0.5f, 0.5f}};
//    arrput(aabbs, aabb);
//
//    // Initialize Lights
//    lights = calloc(1, sizeof(Light));
//    lights[0] = CreateLight(LIGHT_POINT, (Vector3){ -2, 1, -2 }, Vector3Zero(), WHITE, cube_shader);
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

void run(void) {
    // Application loop
    while (!WindowShouldClose()) {
        processInputs();

        const float delta_time = GetFrameTime();

        // Draw
        BeginDrawing();
        ClearBackground(DARKGRAY);

        BeginMode3D(camera);
        draw_grid();

        // Draw models
        for (int i = 0; i < arrlen(models); ++i) {
            DrawModel(models[i], Vector3Zero(), 1.0f, GRAY);
        }

        // Draw debug
        ecs_run(world, ecs_id(draw_planes), delta_time, NULL);
        ecs_run(world, ecs_id(draw_aabbs), delta_time, NULL);

        // Draw scenes

        EndMode3D();

        DrawFPS(20, 20);
        if (paused) DrawText("PAUSED", 20, 50, 20, BLUE);

        EndDrawing();
    }
}

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
