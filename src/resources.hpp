#pragma once

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_render.h>
#include "SDL2_extras.hpp"

typedef enum {
    RES_IMG_CROSS,
    RES_IMG_CIRCLE,
    RES_IMG_BOARD,
    RES_IMG_TOTAL
} AppImageId;

class ImageResource {
public:
    Uint32 id;
    const char* name;
    SDL_Texture* texture;
    SDL_Surface* surface;
    SDL_Size* size;

    ImageResource(SDL_Renderer* renderer, AppImageId id, const char* name, const char* path);
    ~ImageResource();
};
