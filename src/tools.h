//
// Created by josue on 2024-02-26.
//

#ifndef LEARNPHYSICS_TOOLS_H
#define LEARNPHYSICS_TOOLS_H

#include "primitives.h"
#include "raylib.h"

typedef struct AddRectangleParam {
    vec3 position;
    vec3 size;
} AddRectangleParam;

void addRectangleToolInit(Plane base_plane);
bool addRectangleTool(RayPrimitive mouse_ray, Camera camera, AddRectangleParam *param);
void addRectangleToolDraw(void);

#endif //LEARNPHYSICS_TOOLS_H
