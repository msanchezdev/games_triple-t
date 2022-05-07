#pragma once

#include <yaml-cpp/node/node.h>
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
private:
    YAML::Node config;

    void InitializeVideo();
    void InitializeGameWindow();
    void InitializeImages();

    void LoadConfig();
    void LoadResources();

    void Loop();
    void Render();
public:
    SDL_Size window_size = { SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    map<string, ImageResource*> images;
    TRT_EventListener* events = new TRT_EventListener();

    App();
    ~App();

    void Initialize();
    void Start();
    void Quit();

    void LoadImage(string name, string path);
};

extern App app;
