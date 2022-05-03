#pragma once

#include <SDL2/SDL_render.h>
#include <map>
#include <list>
#include <functional>
#include "defs.hpp"
#include "resources.hpp"
#include "events.hpp"

#define App_LoadImage(key) app.LoadImage(#key, __PATH__##key);

using namespace std;

/**
 * Global application object.
 */
class App {
public:
    SDL_Size window_size = { SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    map<const char*, ImageResource*> images;
    TRT_EventListener* events = new TRT_EventListener();

    App();
    ~App();

    void LoadImage(const char* name, const char* path);
    void Cleanup();
};

extern App app;
