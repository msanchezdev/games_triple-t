#pragma once

#include <SDL2/SDL_render.h>
#include <triton/game_object.hpp>
#include <triton/types.hpp>

typedef struct {
    TRT_MouseEnterHandler* on_mouse_enter;
    TRT_MouseLeaveHandler* on_mouse_leave;
    TRT_MouseDownHandler* on_mouse_down;
    TRT_MouseUpHandler* on_mouse_up;
    TRT_MouseMoveHandler* on_mouse_move;
    TRT_MouseOverHandler* on_mouse_over;
} TRT_MouseListenerHandlers;

class TRT_MouseListener2D : public TRT_Component {
public:
    TRT_MouseListenerHandlers handlers;
    void* data;
    TRT_MouseListener2D(TRT_MouseListenerHandlers handlers) : handlers(handlers) {}
    TRT_MouseListener2D(TRT_MouseListenerHandlers handlers, void* data) : handlers(handlers), data(data) {}

    void OnAttach(TRT_GameObject* game_object) override;
    void OnDetach(TRT_GameObject* game_object) override;
};
