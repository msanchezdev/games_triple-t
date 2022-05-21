#pragma once

#include <yaml-cpp/node/node.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
#include <map>
#include <vector>
#include <functional>
#include "types.hpp"
#include "resources.hpp"
#include "events.hpp"

using namespace std;

long double operator "" units(long double value);

long double operator "" units(unsigned long long value);

namespace triton {
    class Camera;

    /**
     * Global application object.
     */
    class App {
    public:
        enum class EventType {
            Render,
            CameraRender,
            MouseMove,
            MouseButtonUp,
            MouseButtonDown,
        };

        typedef struct {} RenderEvent;
        DefineEventHandler(RenderEventHandler, App, RenderEvent);

        typedef struct {
            Camera* camera;
        } CameraRenderEvent;
        DefineEventHandler(CameraRenderEventHandler, App, CameraRenderEvent);

        using MouseEnterEvent = SDL_WindowEvent;
        DefineEventHandler(MouseEnterEventHandler, App, MouseEnterEvent);

        using MouseLeaveEvent = SDL_WindowEvent;
        DefineEventHandler(MouseLeaveEventHandler, App, MouseLeaveEvent);

        using MouseButtonUpEvent = SDL_MouseButtonEvent;
        DefineEventHandler(MouseButtonUpEventHandler, App, MouseButtonUpEvent);

        using MouseButtonDownEvent = SDL_MouseButtonEvent;
        DefineEventHandler(MouseButtonDownEventHandler, App, MouseButtonDownEvent);

        using MouseMoveEvent = SDL_MouseMotionEvent;
        DefineEventHandler(MouseMoveEventHandler, App, App::MouseMoveEvent);

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
        int pixel_per_unit = 100;

        void InitializeVideo();
        void InitializeGameWindow();
        void InitializeImages();
        void InitializeFonts();

        void LoadConfig();
        void LoadResources();

        void Loop();
        void Render();

        friend class Camera;
    public:
        bool fullscreen = false;
        Size resolution = Size(300, 300);
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;

        vector<Camera*> cameras;
        map<string, ImageResource*> images;
        map<string, FontResource*> fonts;
        GameObject root = GameObject("<root>");
        GameObject ui = GameObject("<ui>");
        EventListener<App, EventType> events = EventListener<App, EventType>(this);

        App();
        ~App();

        void Initialize();
        float GetPixelPerUnit();
        int Start();

        void LoadImage(string name, string path);
        void LoadFont(string name, string path);

        Camera* CreateCamera(string name);
    };
}

extern triton::App app;
