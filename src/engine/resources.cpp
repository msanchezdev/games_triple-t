#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include "SDL2_extras.hpp"
#include "resources.hpp"
#include "utils.hpp"

ImageResource::ImageResource(SDL_Renderer* renderer, const char* name, const char* path) {
    debug("Loading image %s: %s", name, path);

    this->name = name;
    surface = IMG_Load(path);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    size = new SDL_Size {
        .w = surface->w,
        .h = surface->h
    };

    if (surface == NULL) {
        error("Unable to load image %s: %s", name, SDL_GetError());
    }
}

ImageResource::~ImageResource() {
    debug("Unloading image %s", name);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
