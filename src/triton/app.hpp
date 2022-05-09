#pragma once

#include <yaml-cpp/node/node.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
#include <map>
#include <vector>
#include <functional>
#include "game_object.hpp"
#include "types.hpp"
#include "resources.hpp"
#include "events.hpp"

#define App_LoadImage(key) app.LoadImage(#key, __PATH__##key);

using namespace std;

namespace triton {
    /**
     * Global application object.
     */
    class App {
    public:
        enum class EventType {
            Render,
            MouseMove,
            MouseButtonUp,
            MouseButtonDown,
        };

        typedef struct {} RenderEvent;
        DefineEventHandler(RenderEventHandler, App, RenderEvent);

        using MouseEnterEvent = SDL_WindowEvent;
        DefineEventHandler(MouseEnterEventHandler, App, MouseEnterEvent);

        using MouseLeaveEvent = SDL_WindowEvent;
        DefineEventHandler(MouseLeaveEventHandler, App, MouseLeaveEvent);

        using MouseUpEvent = SDL_MouseButtonEvent;
        DefineEventHandler(MouseUpEventHandler, App, MouseUpEvent);

        using MouseDownEvent = SDL_MouseButtonEvent;
        DefineEventHandler(MouseDownEventHandler, App, MouseDownEvent);

        using MouseMoveEvent = SDL_MouseMotionEvent;
        DefineEventHandler(MouseMoveEventHandler, App, App::MouseMoveEvent);

        // TODO: implement on render with global and per event data
        typedef struct {
            Uint32 timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
            Uint32 windowID;    /**< The window with mouse focus, if any */
            Uint32 state;       /**< The current button state */
            Sint32 x;           /**< X coordinate, relative to window */
            Sint32 y;           /**< Y coordinate, relative to window */
        } MouseOverEvent;
        DefineEventHandler(MouseOverEventHandler, App, void);

    private:
        YAML::Node config;

        void InitializeVideo();
        void InitializeGameWindow();
        void InitializeImages();

        void LoadConfig();
        void LoadResources();

        void Loop();
        void Render();
        map<string, GameObject*> game_objects;

    public:
        bool fullscreen;
        Size resolution {
            300,
            300
        };
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;

        map<string, ImageResource*> images;
        GameObject root = GameObject("<root>");
        EventListener<App, EventType> events = EventListener<App, EventType>(this);

        App();
        ~App();

        void Initialize();
        void Start();
        void Quit();

        void LoadImage(string name, string path);

        // Game Objects
        void AddGameObject(GameObject* game_object);
        GameObject* GetGameObject(string name);
        void RemoveGameObject(string name);
        bool HasGameObject(string name);
    };
}

extern triton::App app;
