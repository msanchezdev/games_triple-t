#pragma once

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_render.h>
#include <string>
#include "SDL2_extras.hpp"

using namespace std;

namespace triton {
    class ImageResource {
    public:
        string name;
        SDL_Texture* texture;
        SDL_Surface* surface;
        SDL_Size* size;

        ImageResource(SDL_Renderer* renderer, string name, string path);
        ~ImageResource();
    };
}
