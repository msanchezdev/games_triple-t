#pragma once

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_render.h>
#include "SDL2_extras.hpp"

class ImageResource {
public:
    const char* name;
    SDL_Texture* texture;
    SDL_Surface* surface;
    SDL_Size* size;

    ImageResource(SDL_Renderer* renderer, const char* name, const char* path);
    ~ImageResource();
};
