#pragma once

#include <SDL2/SDL_render.h>
#include "resources.hpp"

typedef struct {
    SDL_Size window_size;
    SDL_Window* window;
    SDL_Renderer* renderer;
    ImageResource* images[AppImageId::RES_IMG_TOTAL];
} App;

extern App app;
