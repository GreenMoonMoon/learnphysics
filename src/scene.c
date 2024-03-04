//
// Created by josue on 2024-02-27.
//

#include "scene.h"

// Scene description
/*
 * number of objects
 * objects meshes
 *  - type
 *      - primitive
 *      - resource
 * objects material
 *  - shader
 *  - values
 * objects transform
 * objects bounding volume (could be very specific to the game to avoid overhead)
 * number of lights
 * light transform
 * light material
 *  - color
 */

// RESOURCE STORES
// Resource store is contains all the loaded shared resources.
// TODO: handle stores using Flecs?

// Mesh Store

// Material Store


enum MeshType {
    MESHTYPE_PRIMITIVE,
    MESHTYPE_RESOURCE
};

enum MeshPrimitive {
    MESHPRIMITIVE_CUBE
};

typedef struct MeshDescription {
    enum MeshType type;
    union {
        enum MeshPrimitive primitive;
        int resource_id;
    };
} MeshDescription;

typedef struct MaterialDescription {
    // Material description ...
} MaterialDescription;

typedef struct ModelDescription {
    MeshDescription mesh;
    MaterialDescription material;
    mat4 transform;
} ModelDescription;

enum LightType {
    LIGHTTYPE_POINT,
    LIGHTTYPE_DIRECTIONAL,
    LIGHTTYPE_SPOT
};

union LightParameters {
    struct {
        vec3 position;
        float radius;
    } point;
    struct {
        vec3 direction;
    } directional;
    struct {
        vec3 position;
        vec3 direction;
        float angle;
    } spot;
} LightParameters;

typedef struct LightDescription {
    enum LightType type;
    union LightParameters parameters;
    vec3 color;
    float intensity;
} LightDescription;

static void mesh_load(const MeshDescription mesh_description) {
    switch(mesh_description.type){
        case MESHTYPE_PRIMITIVE:
            switch(mesh_description.primitive) {
                case MESHPRIMITIVE_CUBE:
                    break;
            }
            break;
        case MESHTYPE_RESOURCE:
            // load resource
            // mesh_description.resource_id;
            break;
    }
}

static void material_load(const MaterialDescription material_description) {

}

static void light_load(const LightDescription light_description) {
    switch (light_description.type) {
        case LIGHTTYPE_POINT:
            break;
        case LIGHTTYPE_DIRECTIONAL:
            break;
        case LIGHTTYPE_SPOT:
            break;
    }
}

ecs_entity_t scene3D_load(const SceneDescription scene_description) {
    ecs_entity_t scene_id;

    // Load a series of cubes based on a description
    for(int i = 0; i < scene_description.model_count; ++i) {
        // Load mesh;
        mesh_load(scene_description.models[i].mesh);
        // Load material;
        material_load(scene_description.models[i].material);
        // Set transform;
    }

    // Load lights
    for (int i = 0; i < scene_description.light_count; ++i) {
        light_load(scene_description.lights[i]);
    }

    return scene_id;
}
