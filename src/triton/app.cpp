#include <map>
#include <list>
#include <functional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <yaml-cpp/yaml.h>
#include "defs.hpp"
#include "app.hpp"
#include "utils.hpp"

App app;

App::App() {}

App::~App() {}

void App::Initialize() {
    this->LoadConfig();

    this->InitializeVideo();
    this->InitializeGameWindow();
    this->InitializeImages();

    this->LoadResources();
}

void App::Start() {
    this->Loop();
}

void App::Quit() {
    debug("Shutting down the application");

    debug("Unloading images");
    this->images.clear();

    debug("Unregistering event handlers");
    delete events;

    debug("Destroying renderer");
    SDL_DestroyRenderer(this->renderer);

    debug("Closing main window");
    SDL_DestroyWindow(window);

    debug("Stopping image subsystem");
    IMG_Quit();

    debug("Stopping SDL subsystems");
    SDL_Quit();
}

void App::InitializeVideo() {
    debug("Initializing video subsystem");

    if (SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        critical("Unable to initialize video subsystem: %s", SDL_GetError());
        exit(EXIT_CODE_ERROR);
    }

    debug("Video subsystem initialized successfully");
}

void App::InitializeGameWindow() {
    debug("Creating game window");
    this->window = SDL_CreateWindow(
        "Triple T",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_NO_FLAGS
    );

    if (this->window == nullptr) {
        critical("Unable to open game window: %s", SDL_GetError());
        exit(EXIT_CODE_ERROR);
    }

    debug("Creating main renderer");
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED);

    if (this->renderer == nullptr) {
        critical("Unable to create main renderer: %s", SDL_GetError());
        exit(EXIT_CODE_ERROR);
    }

    debug("Game window opened successfully");
}

void App::InitializeImages() {
    debug("Initializing image subsystem");
    int formatLoaded = IMG_Init(IMG_INIT_PNG);

    if (!formatLoaded & IMG_INIT_PNG) {
        critical("Unable to initialize image subsystem: %s", IMG_GetError());
        exit(EXIT_CODE_ERROR);
    }

    debug("Image subsystem initialized successfully");
}

void App::Loop() {
    debug("Entering main loop");

    SDL_Event event;
    while (true) {
        if (!SDL_PollEvent(&event)) continue;

        this->Render();

        switch (event.type) {
        case SDL_EventType::SDL_QUIT:
            debug("Event Received: Quit");
            return;

        case SDL_EventType::SDL_KEYDOWN:
            if (event.key.keysym.sym == SDL_KeyCode::SDLK_ESCAPE) {
                debug("Event Received: Key down, Escape");
                debug("Pressed Escape, exitting");
                return;
            }
            break;

        case SDL_EventType::SDL_MOUSEMOTION:
            this->events->Broadcast(TRT_EventType::EVENT_MOUSE_MOVE, &event.motion);
            break;

        case SDL_EventType::SDL_MOUSEBUTTONDOWN:
            this->events->Broadcast(TRT_EventType::EVENT_MOUSE_DOWN, &event.button);
            break;

        case SDL_EventType::SDL_MOUSEBUTTONUP:
            this->events->Broadcast(TRT_EventType::EVENT_MOUSE_UP, &event.button);
            break;

        default:
            debug("Event Received: %#x", event.type);
            break;
        }
    }
}

void App::Render() {
    SDL_Rect windowRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_SetRenderDrawColor(app.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(this->renderer);

    this->events->Broadcast(TRT_EventType::EVENT_RENDER);

    SDL_RenderPresent(this->renderer);
}

void App::LoadImage(string name, string path) {
    this->images[name] = new ImageResource(this->renderer, name, path);
}

void App::LoadConfig() {
    this->config = YAML::LoadFile("triton.yaml");

    // Set log level
    if (auto loglevel = this->config["loglevel"]) {
        string level_str = this->config["loglevel"].as<string>();
        SDL_LogPriority new_loglevel = SDL_LOG_PRIORITY_CRITICAL;

        if (level_str == "verbose") {
            new_loglevel = SDL_LOG_PRIORITY_VERBOSE;
        } else if (level_str == "debug") {
            new_loglevel = SDL_LOG_PRIORITY_DEBUG;
        } else if (level_str == "info") {
            new_loglevel = SDL_LOG_PRIORITY_INFO;
        } else if (level_str == "warn") {
            new_loglevel = SDL_LOG_PRIORITY_WARN;
        } else if (level_str == "error") {
            new_loglevel = SDL_LOG_PRIORITY_ERROR;
        } else if (level_str == "critical") {
            new_loglevel = SDL_LOG_PRIORITY_CRITICAL;
        } else {
            throw std::runtime_error("Invalid value for 'loglevel'. Valid levels are: verbose, debug, info, warn, error, critical");
        }

        SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, new_loglevel);
    }
}

void App::LoadResources() {
    if (auto assets = this->config["assets"]) {
        if (!assets.IsMap()) {
            throw std::runtime_error("Invalid value for 'assets', expected map");
        }

        if (auto images = assets["images"]) {
            if (!images.IsMap()) {
                throw std::runtime_error("Invalid 'assets.images' config, expected map");
            }

            for (auto image : images) {
                string name = image.first.as<string>();
                string path = image.second.as<string>();

                this->LoadImage(name, "assets/images/" + path);
            }
        }
    }
}
