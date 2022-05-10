#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "SDL2_extras.hpp"
#include "resources.hpp"
#include "utils.hpp"

using namespace std;
using namespace triton;

ImageResource::ImageResource(SDL_Renderer* renderer, string name, string path) : name(name) {
    debug("Loading image %s: %s", name.c_str(), path.c_str());

    surface = IMG_Load(path.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    size = new SDL_Size {
        .w = surface->w,
        .h = surface->h
    };

    if (surface == nullptr) {
        error("Unable to load image %s: %s", name.c_str(), SDL_GetError());
        surface = SDL_CreateRGBSurface(0, 0, 0, 0, 0, 0, 0, 0);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        size = new SDL_Size {
            .w = 0,
            .h = 0
        };
    }
}

ImageResource::~ImageResource() {
    debug("Unloading image %s", name.c_str());
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


FontResource::FontResource(string name, string path) : name(name) {
    debug("Loading font %s: %s", name.c_str(), path.c_str());

    font = TTF_OpenFont(path.c_str(), 0);

    if (font == nullptr) {
        error("Unable to load font %s: %s", name.c_str(), TTF_GetError());
    }
}

FontResource::~FontResource() {
    debug("Unloading font %s", name.c_str());
    TTF_CloseFont(font);
}
