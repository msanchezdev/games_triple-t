#pragma once

#include <SDL2/SDL_render.h>
#include <triton/game_object.hpp>
#include <triton/types.hpp>

namespace triton {
    enum class MouseListener_EventType {
        MouseEnter,
        MouseLeave,
        MouseDown,
        MouseUp,
        MouseMove,
        MouseOver
    };

    class MouseListener : public Component, public WithEventListener<MouseListener, MouseListener_EventType> {
    public:
        using EventType = MouseListener_EventType;

        typedef struct {} MouseEnterEvent;
        DefineEventHandler(MouseEnterEventHandler, MouseListener, void*);

        typedef struct {} MouseLeaveEvent;
        DefineEventHandler(MouseLeaveEventHandler, MouseListener, void*);

        typedef struct {} MouseDownEvent;
        DefineEventHandler(MouseDownEventHandler, MouseListener, void*);

        typedef struct {} MouseUpEvent;
        DefineEventHandler(MouseUpEventHandler, MouseListener, void*);

        typedef struct {} MouseMoveEvent;
        DefineEventHandler(MouseMoveEventHandler, MouseListener, void*);

        typedef struct {} MouseOverEvent;
        DefineEventHandler(MouseOverEventHandler, MouseListener, void*);

    private:
        friend void MouseListener_MouseOverChecker(EventArgs<App, MouseListener*, App::RenderEvent*>* event);
        friend void MouseListener_MouseUpChecker(EventArgs<App, MouseListener*, App::MouseUpEvent*>* event);
        friend void MouseListener_MouseDownChecker(EventArgs<App, MouseListener*, App::MouseDownEvent*>* event);
        friend void MouseListener_MouseChecker(EventArgs<App, MouseListener*, App::MouseMoveEvent*>* event);

    public:
        Rect* check_rect;
        bool is_inside;

        // Arbitrary data that can be used by the event handlers
        void* data;

        MouseListener(Rect* check_rect) : check_rect(check_rect), WithEventListener(this) {}
        MouseListener(Rect* check_rect, void* data) : check_rect(check_rect), data(data), WithEventListener(this) {}

        void OnAttach(GameObject* game_object) override;
        void OnDetach(GameObject* game_object) override;
    };
}
