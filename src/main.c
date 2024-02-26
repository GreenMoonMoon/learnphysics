#include "raylib.h"
#include "raylib_utils.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#include "cglm/cglm.h"

#include "gizmos.h"
#include "collision.h"

#define STB_DS_IMPLEMENTATION
#include "utils/stb_ds.h"

#define FRAND(A) (float)rand()/(float)(RAND_MAX/A)

static Camera3D camera;
static bool paused;

static Matrix *cubes = NULL;
static Mesh cube_mesh;
static Material cube_material;

const Plane ground_plane = {.normal = {0.0f, 1.0f, 0.0f}, .distance = 0.0f};

// TEST GIZMOS
static struct {
    AABB rectangle;
    Plane plane;
    vec3 intersection_point;
    uint8_t step; // step == 0 is inactive
} add_rectangle_tool = {0};

void instanciateCube(vec3 position, vec3 size) {
    Matrix transform = MatrixIdentity();
    transform = MatrixMultiply(transform, MatrixScale(size[0], size[1], size[2]));
    transform = MatrixMultiply(transform, MatrixTranslate(position[0], position[1], position[2]));

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
    // Get some required shader locations
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
    if (!add_rectangle_tool.step) { // Activate tool
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            get_ray_plane_intersection(mouse_ray, ground_plane, add_rectangle_tool.intersection_point)) {
            add_rectangle_tool.step++; // Go to the initial "active" step.
            // Initial values
            add_rectangle_tool.plane = ground_plane;
            glm_vec3_copy(add_rectangle_tool.intersection_point,
                          add_rectangle_tool.rectangle.min);
            glm_vec3_copy(add_rectangle_tool.intersection_point,
                          add_rectangle_tool.rectangle.max);
        }
    } else {
        const bool intersects = get_ray_plane_intersection(mouse_ray,
                                                           add_rectangle_tool.plane,
                                                           add_rectangle_tool.intersection_point);
        switch (add_rectangle_tool.step) {
            case 1:
                if (intersects) {
                    add_rectangle_tool.rectangle.max[0] = add_rectangle_tool.intersection_point[0];
                    add_rectangle_tool.rectangle.max[2] = add_rectangle_tool.intersection_point[2];
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) { // Mouse on to the next step
                    add_rectangle_tool.step++;
                    // Move from xz plane to xy plane for height
                    add_rectangle_tool.plane = (Plane){{0.0f, 0.0f, 1.0f}, add_rectangle_tool.rectangle.max[2]};
                }
                break;
            case 2: // last step
            default:
                if (intersects) {
                    add_rectangle_tool.rectangle.max[1] = add_rectangle_tool.intersection_point[1];
                }
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    add_rectangle_tool.step = 0; // clear tool
                    vec3 size;
                    glm_vec3_sub(add_rectangle_tool.rectangle.max, add_rectangle_tool.rectangle.min, size);
                    instanciateCube(add_rectangle_tool.rectangle.min, size);
                }
                break;
        }
    }
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

        // Draw "Add Rectangle" gizmo
        if (add_rectangle_tool.step) {
            draw_aabb_on_plane_gizmo(add_rectangle_tool.plane, add_rectangle_tool.rectangle, GREEN);
        }

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
