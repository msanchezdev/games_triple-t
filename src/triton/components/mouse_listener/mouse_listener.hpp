#pragma once

#include <SDL2/SDL_render.h>
#include <triton/game_object.hpp>
#include <triton/types.hpp>

enum class TRT_MouseListener2D_EventType {
    MouseEnter,
    MouseLeave,
    MouseDown,
    MouseUp,
    MouseMove,
    MouseOver
};

class TRT_MouseListener2D : public TRT_Component, public TRT_WithEventListener<TRT_MouseListener2D, TRT_MouseListener2D_EventType> {
public:
    using EventType = TRT_MouseListener2D_EventType;

    typedef struct {} MouseEnterEvent;
    DefineEventHandler(MouseEnterEventHandler, TRT_MouseListener2D, void*);

    typedef struct {} MouseLeaveEvent;
    DefineEventHandler(MouseLeaveEventHandler, TRT_MouseListener2D, void*);

    typedef struct {} MouseDownEvent;
    DefineEventHandler(MouseDownEventHandler, TRT_MouseListener2D, void*);

    typedef struct {} MouseUpEvent;
    DefineEventHandler(MouseUpEventHandler, TRT_MouseListener2D, void*);

    typedef struct {} MouseMoveEvent;
    DefineEventHandler(MouseMoveEventHandler, TRT_MouseListener2D, void*);

    typedef struct {} MouseOverEvent;
    DefineEventHandler(MouseOverEventHandler, TRT_MouseListener2D, void*);

private:
    friend void TRT_MouseListener2D_MouseOverChecker(TRT_EventArgs<App, TRT_MouseListener2D*, App::RenderEvent*>* event);
    friend void TRT_MouseListener2D_MouseUpChecker(TRT_EventArgs<App, TRT_MouseListener2D*, App::MouseUpEvent*>* event);
    friend void TRT_MouseListener2D_MouseDownChecker(TRT_EventArgs<App, TRT_MouseListener2D*, App::MouseDownEvent*>* event);
    friend void TRT_MouseListener2D_MouseChecker(TRT_EventArgs<App, TRT_MouseListener2D*, App::MouseMoveEvent*>* event);

public:
    TRT_Rect2D* check_rect;
    bool is_inside;

    // Arbitrary data that can be used by the event handlers
    void* data;

    TRT_MouseListener2D(TRT_Rect2D* check_rect) : check_rect(check_rect), TRT_WithEventListener(this) {}
    TRT_MouseListener2D(TRT_Rect2D* check_rect, void* data) : check_rect(check_rect), data(data), TRT_WithEventListener(this) {}

    void OnAttach(TRT_GameObject* game_object) override;
    void OnDetach(TRT_GameObject* game_object) override;
};
