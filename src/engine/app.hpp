#pragma once

#include <SDL2/SDL_render.h>
#include <map>
#include "resources.hpp"

#define App_LoadImage(key) app.LoadImage(#key, __PATH__##key);

/**
 * Global application object.
 */
class App {
public:
    SDL_Size window_size;
    SDL_Window* window;
    SDL_Renderer* renderer;

    std::map<const char*, ImageResource*> images;

    App();
    ~App();

    void LoadImage(const char* name, const char* path);
};

extern App app;
