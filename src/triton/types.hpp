#pragma once

typedef struct {
    int x;
    int y;
} TRT_Point2D;

typedef struct {
    int width;
    int height;
} TRT_Size2D;

typedef struct {
    TRT_Point2D* position;
    TRT_Size2D* size;

    // implicit conversion to SDL_Rect


} TRT_Rect2D;
