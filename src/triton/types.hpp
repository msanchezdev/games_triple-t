#pragma once
#include <SDL2/SDL_rect.h>

namespace triton {
    typedef struct Vector {
        int x;
        int y;

        Vector() : x(0), y(0) {}

        Vector(int x, int y) : x(x), y(y) {}

        operator SDL_Point() {
            return SDL_Point { x, y };
        }
    } Vector;

    typedef struct Size {
        int width;
        int height;

        Size() : width(0), height(0) {}

        Size(int width, int height) : width(width), height(height) {}

        operator SDL_Rect() {
            return SDL_Rect { 0, 0, width, height };
        }
    } Size;

    typedef struct Rect {
        int x;
        int y;
        int width;
        int height;

        Rect() : x(0), y(0), width(0), height(0) {}

        Rect(int x, int y, int width, int height)
            : x(x), y(y), width(width), height(height) {
        }

        Rect(Vector* position, Size* size)
            : x(position->x), y(position->y), width(size->width), height(size->height) {
        }

        Vector* GetVector() {
            return new Vector(x, y);
        }

        Size* GetSize() {
            return new Size(width, height);
        }

        operator SDL_Rect() {
            return SDL_Rect {
                .x = x,
                .y = y,
                .w = width,
                .h = height
            };
        }
    } Rect;
}
