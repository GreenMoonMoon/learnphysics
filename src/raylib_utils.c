//
// Created by josue on 2024-01-24.
//
#include "raylib_utils.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

void set_line_width(float width) {
    rlSetLineWidth(width);
}

void draw_grid(void) {
    DrawGrid(16, 1);
    DrawLine3D(Vector3Zero(), (Vector3){5.0f, 0.0f, 0.0f}, RED);
    DrawLine3D(Vector3Zero(), (Vector3){0.0f, 5.0f, 0.0f}, GREEN);
    DrawLine3D(Vector3Zero(), (Vector3){0.0f, 0.0f, 5.0f}, BLUE);
}
