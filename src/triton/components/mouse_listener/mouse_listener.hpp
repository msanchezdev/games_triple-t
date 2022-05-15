#pragma once

#include <SDL2/SDL_render.h>
#include <triton/app.hpp>
#include <triton/game_object.hpp>
#include <triton/types.hpp>

namespace triton {
    enum class MouseListener_EventType {
        MouseEnter,
        MouseLeave,
        MouseButtonDown,
        MouseButtonUp,
        MouseMove,
        MouseOver
    };

    class MouseListener : public Component, public WithEventListener<MouseListener, MouseListener_EventType> {
    private:
        Rect* check_rect;
        bool is_inside;

        static void MouseOverChecker(EventArgs<App, MouseListener, App::RenderEvent>* event);
        static void MouseButtonDownChecker(EventArgs<App, MouseListener, App::MouseButtonDownEvent>* event);
        static void MouseButtonUpChecker(EventArgs<App, MouseListener, App::MouseButtonUpEvent>* event);
        static void MouseChecker(EventArgs<App, MouseListener, App::MouseMoveEvent>* event);

        void OnEnable(GameObject* game_object) override;
        void OnDisable(GameObject* game_object) override;

    public:
        using EventType = MouseListener_EventType;

        typedef struct {} MouseEnterEvent;
        DefineEventHandler(MouseEnterEventHandler, MouseListener, void*);

        typedef struct {} MouseLeaveEvent;
        DefineEventHandler(MouseLeaveEventHandler, MouseListener, void*);

        typedef struct {} MouseButtonDownEvent;
        DefineEventHandler(MouseButtonDownEventHandler, MouseListener, void*);

        typedef struct {} MouseButtonUpEvent;
        DefineEventHandler(MouseButtonUpEventHandler, MouseListener, void*);

        typedef struct {} MouseMoveEvent;
        DefineEventHandler(MouseMoveEventHandler, MouseListener, void*);

        typedef struct {} MouseOverEvent;
        DefineEventHandler(MouseOverEventHandler, MouseListener, void*);

        // Arbitrary data that can be used by the event handlers
        void* data;

        MouseListener(Rect* check_rect) : check_rect(check_rect), WithEventListener(this) {}
        MouseListener(Rect* check_rect, void* data) : check_rect(check_rect), data(data), WithEventListener(this) {}
    };
}
