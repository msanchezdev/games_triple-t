#pragma once

#include <SDL2/SDL_rect.h>
#include "types/vector.hpp"
#include "types/component.hpp"
#include "types/game_object.hpp"

namespace triton {

    typedef struct Size {
        float width;
        float height;

        Size() : width(0), height(0) {}

        Size(float width, float height) : width(width), height(height) {}

        operator SDL_Rect() {
            return SDL_Rect { 0, 0, (int)width, (int)height };
        }

        operator SDL_FRect() {
            return SDL_FRect { 0, 0, width, height };
        }
    } Size;

    typedef struct Rect {
        float x;
        float y;
        float width;
        float height;

        Rect() : x(0), y(0), width(0), height(0) {}

        Rect(float x, float y, float width, float height)
            : x(x), y(y), width(width), height(height) {
        }

        Rect(Vector* position, Size* size)
            : x(position->x), y(position->y), width(size->width), height(size->height) {
        }

        Vector* GetVector() {
            return new Vector(x, y, 0);
        }

        Size* GetSize() {
            return new Size(width, height);
        }

        operator SDL_Rect() {
            return SDL_Rect {
                .x = (int)x,
                .y = (int)y,
                .w = (int)width,
                .h = (int)height
            };
        }

        operator SDL_FRect() {
            return SDL_FRect {
                .x = x,
                .y = y,
                .w = width,
                .h = height
            };
        }
    } Rect;

    using Color = SDL_Color;
}
