#include "raylib.h"
#include "raylib_utils.h"
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#include "flecs.h"

#include "collision.h"
#include "bvh.h"

#define STB_DS_IMPLEMENTATION
#include "utils/stb_ds.h"

#include "graphic/draw_primitive.h"

static Camera3D camera;
static ecs_world_t *world;
static BVH bvh;

// Declare components
ECS_COMPONENT_DECLARE(Plane);
ECS_COMPONENT_DECLARE(AABB);
ECS_COMPONENT_DECLARE(Color);

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

    // Add BVH to the world
    bvh_init();

    // Add a few cubes
    ecs_entity_t cube_a = ecs_new_id(world);
    ecs_set(world, cube_a, AABB, {.min = {0.0f, 0.0f, 0.0f}, .max = {1.0f, 1.0f, 1.0f}});
    ecs_entity_t cube_b = ecs_new_id(world);
    ecs_set(world, cube_b, AABB, {.min = {2.0f, 1.0f, 3.0f}, .max = {3.0f, 2.0f, 5.0f}});

    ecs_entity_t union_ab = ecs_new_id(world);
    ecs_add(world, union_ab, AABB);
    *ecs_get_mut(world, union_ab, AABB) = aabb_union(*ecs_get(world, cube_a, AABB), *ecs_get(world, cube_b, AABB));
    ecs_set(world, union_ab, Color, { 230, 41, 55, 255 });

//    ecs_entity_t cube_d = ecs_new_id(world);
//    ecs_set(world, cube_d, AABB, {.min = {0.0f, 0.0f, 0.0f}, .max = {1.0f, 1.0f, 1.0f}});
//    ecs_entity_t cube_e = ecs_new_id(world);
//    ecs_set(world, cube_e, AABB, {.min = {0.0f, 0.0f, 0.0f}, .max = {1.0f, 1.0f, 1.0f}});

    // Load lights
    Light light_a = init_light(LIGHT_POINT, (Vector3){ -2, 1, -2 }, Vector3Zero(), WHITE);
}

void draw_planes(ecs_iter_t *it) {
    Plane *p = ecs_field(it, Plane, 1);

    for (int i = 0; i < it->count; ++i) {
        plane_draw(p[i], GREEN);
    }
}

void draw_aabbs(ecs_iter_t *it) {
    AABB *a = ecs_field(it, AABB, 1);

    if(ecs_field_is_set(it, 2)) {
        Color *c = ecs_field(it, Color, 2);
        for (int i = 0; i < it->count; ++i) {
            aabb_draw(a[i], c[i], 0.1f);
        }
    } else {
        for (int i = 0; i < it->count; ++i) {
            aabb_draw(a[i], BLUE, 0.0f);
        }
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
    ECS_COMPONENT_DEFINE(world, Color);

    // Initialize tags
    ECS_TAG(world, MainCamera);

    // Initialize systems
    ECS_SYSTEM_DEFINE(world, draw_planes, EcsOnStore, [in]Plane);
    ECS_SYSTEM_DEFINE(world, draw_aabbs, EcsOnStore, [in]AABB, ?Color);

    // Create camera
    camera = (Camera3D) {
            .position = {0.0f, 3.0f, -8.0f},
            .up = {0.0f, 1.0f, 0.0f},
            .target = {0},
            .fovy = 70,
            .projection = CAMERA_PERSPECTIVE
    };

    paused = true;

    // SCENE
    load_basic_scene();

    // Create the binary tree and sort element into it.
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
