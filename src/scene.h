//
// Created by josue on 2024-02-27.
//

#ifndef SCENE_H
#define SCENE_H

#include "cglm/cglm.h"
#include "flecs.h"

struct ModelDescription;
struct LightDescription;

enum SceneType {
    SCENETYPE_3D,
    SCENETYPE_UI
};

typedef struct SceneDescription {
    enum SceneType type;
    int model_count;
    struct ModelDescription *models;
    int light_count;
    struct LightDescription *lights;
} SceneDescription;

ecs_entity_t scene3D_load(SceneDescription scene_description);

void scene3D_unload(ecs_entity_t scene);

void scene_draw_system(void);

#endif //SCENE_H
