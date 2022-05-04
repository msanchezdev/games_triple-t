/**
 * TODO: Move core logic to app.cpp
 */
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "core.hpp"
#include "game.hpp"
#include "SDL2_extras.hpp"
#include "defs.hpp"
#include "utils.hpp"
#include "resources.hpp"

App app;

void TRT_Initialize() {
    TRT_InitializeVideo();
    TRT_InitializeGameWindow();
    TRT_InitializeImages();

    Game_Resources();
    Game_Init();
}

void TRT_InitializeVideo() {
    debug("Initializing video subsystem");

    if (SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        critical("Unable to initialize video subsystem: %s", SDL_GetError());
        exit(EXIT_CODE_ERROR);
    }

    debug("Video subsystem initialized successfully");
}

void TRT_InitializeGameWindow() {
    debug("Creating game window");
    app.window = SDL_CreateWindow(
        "Triple T",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_NO_FLAGS
    );

    if (app.window == nullptr) {
        critical("Unable to open game window: %s", SDL_GetError());
        exit(EXIT_CODE_ERROR);
    }

    debug("Creating main renderer");
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED);

    if (app.renderer == nullptr) {
        critical("Unable to create main renderer: %s", SDL_GetError());
        exit(EXIT_CODE_ERROR);
    }

    debug("Game window opened successfully");
}

void TRT_InitializeImages() {
    debug("Initializing image subsystem");
    int formatLoaded = IMG_Init(IMG_INIT_PNG);

    if (!formatLoaded & IMG_INIT_PNG) {
        critical("Unable to initialize image subsystem: %s", IMG_GetError());
        exit(EXIT_CODE_ERROR);
    }

    debug("Image subsystem initialized successfully");
}

void TRT_RenderLoop() {
    SDL_Rect windowRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_SetRenderDrawColor(app.renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(app.renderer, &windowRect);

    app.events->Broadcast(TRT_EventType::EVENT_RENDER);

    SDL_RenderPresent(app.renderer);
    SDL_RenderClear(app.renderer);
}

void TRT_EventLoop() {
    debug("Starting event loop");

    while (true) {
        TRT_RenderLoop();

        SDL_Event event;
        if (!SDL_PollEvent(&event)) continue;

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
            app.events->Broadcast(TRT_EventType::EVENT_MOUSE_MOVE, &event.motion);
            break;

        case SDL_EventType::SDL_MOUSEBUTTONDOWN:
            app.events->Broadcast(TRT_EventType::EVENT_MOUSE_DOWN, &event.button);
            break;

        case SDL_EventType::SDL_MOUSEBUTTONUP:
            app.events->Broadcast(TRT_EventType::EVENT_MOUSE_UP, &event.button);
            break;

        default:
            debug("Event Received: %#x", event.type);
            break;
        }
    }
}

void TRT_Shutdown() {
    debug("Shutting down the application");

    app.Cleanup();

    debug("Stopping image subsystem");
    IMG_Quit();

    debug("Stopping SDL subsystems");
    SDL_Quit();
}

void TRT_Debug_PrintInitializedSubsystems() {
    Uint32 subsystems = SDL_WasInit(0);

    debug("Timer: %s", flag_status_str(subsystems & SDL_INIT_TIMER));
    debug("Events: %s", flag_status_str(subsystems & SDL_INIT_EVENTS));
    debug("Game Controller: %s", flag_status_str(subsystems & SDL_INIT_GAMECONTROLLER));
    debug("Haptic: %s", flag_status_str(subsystems & SDL_INIT_HAPTIC));
    debug("Joystick: %s", flag_status_str(subsystems & SDL_INIT_JOYSTICK));
    debug("Sensor: %s", flag_status_str(subsystems & SDL_INIT_SENSOR));
    debug("Video: %s", flag_status_str(subsystems & SDL_INIT_VIDEO));
    debug("Audio: %s", flag_status_str(subsystems & SDL_INIT_AUDIO));
}
