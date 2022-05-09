// TODO: Add data to events
#include <csignal>
#include <SDL2/SDL_timer.h>
#include <triton/app.hpp>
#include <triton/components/sprite/sprite.hpp>
#include "mouse_listener.hpp"

void TRT_MouseListener2D_MouseChecker(TRT_EventArgs<App, TRT_MouseListener2D, App::MouseMoveEvent>* event) {
    auto listener = event->reference;
    SDL_Point mouse_pos = { event->data->x, event->data->y };
    SDL_Rect listener_rect = {
        listener->check_rect->position->x, listener->check_rect->position->y,
        listener->check_rect->size->width, listener->check_rect->size->height
    };

    if (SDL_PointInRect(&mouse_pos, &listener_rect)) {
        if (!listener->is_inside) {
            listener->is_inside = true;

            TRT_MouseListener2D::MouseEnterEvent enter_event;
            listener->Emit(TRT_MouseListener2D::EventType::MouseEnter, &enter_event);
        } else {
            TRT_MouseListener2D::MouseMoveEvent move_event;
            listener->Emit(TRT_MouseListener2D::EventType::MouseMove, &move_event);
        }
    } else if (listener->is_inside) {
        listener->is_inside = false;

        TRT_MouseListener2D::MouseLeaveEvent leave_event;
        listener->Emit(TRT_MouseListener2D::EventType::MouseLeave, &leave_event);
    }
}

void TRT_MouseListener2D_MouseDownChecker(TRT_EventArgs<App, TRT_MouseListener2D, App::MouseDownEvent>* event) {
    auto listener = event->reference;
    SDL_Point mouse_pos = { event->data->x, event->data->y };
    SDL_Rect listener_rect = {
        listener->check_rect->position->x, listener->check_rect->position->y,
        listener->check_rect->size->width, listener->check_rect->size->height
    };

    if (SDL_PointInRect(&mouse_pos, &listener_rect)) {
        TRT_MouseListener2D::MouseDownEvent down_event;
        listener->Emit(TRT_MouseListener2D::EventType::MouseDown, &down_event);
    }
}

void TRT_MouseListener2D_MouseUpChecker(TRT_EventArgs<App, TRT_MouseListener2D, App::MouseUpEvent>* event) {
    auto listener = event->reference;
    SDL_Point mouse_pos = { event->data->x, event->data->y };
    SDL_Rect listener_rect = {
        listener->check_rect->position->x, listener->check_rect->position->y,
        listener->check_rect->size->width, listener->check_rect->size->height
    };

    if (SDL_PointInRect(&mouse_pos, &listener_rect)) {
        TRT_MouseListener2D::MouseEnterEvent up_event;
        listener->Emit(TRT_MouseListener2D::EventType::MouseUp, &up_event);
    }
}

void TRT_MouseListener2D_MouseOverChecker(TRT_EventArgs<App, TRT_MouseListener2D, App::RenderEvent>* event) {
    auto listener = event->reference;

    if (listener->is_inside) {
        TRT_MouseListener2D::MouseOverEvent over_event;
        // over_event.state = SDL_GetMouseState(&over_event.x, &over_event.y);
        // over_event.timestamp = SDL_GetTicks();
        // over_event.windowID = SDL_GetWindowID(app.window);
        listener->Emit(TRT_MouseListener2D::EventType::MouseOver, &over_event);
    }
}

void TRT_MouseListener2D::OnAttach(TRT_GameObject* game_object) {
    app.events.Subscribe(App::EventType::MouseMove, new App::MouseMoveEventHandler(TRT_MouseListener2D_MouseChecker, this));
    app.events.Subscribe(App::EventType::MouseButtonDown, new App::MouseDownEventHandler(TRT_MouseListener2D_MouseDownChecker, this));
    app.events.Subscribe(App::EventType::MouseButtonUp, new App::MouseUpEventHandler(TRT_MouseListener2D_MouseUpChecker, this));
    app.events.Subscribe(App::EventType::Render, new App::RenderEventHandler(TRT_MouseListener2D_MouseOverChecker, this));
}

void TRT_MouseListener2D::OnDetach(TRT_GameObject* game_object) {
    app.events.Unsubscribe(App::EventType::MouseMove, new App::MouseMoveEventHandler(TRT_MouseListener2D_MouseChecker, this));
    app.events.Unsubscribe(App::EventType::MouseButtonDown, new App::MouseDownEventHandler(TRT_MouseListener2D_MouseDownChecker, this));
    app.events.Unsubscribe(App::EventType::MouseButtonUp, new App::MouseUpEventHandler(TRT_MouseListener2D_MouseUpChecker, this));
    app.events.Unsubscribe(App::EventType::Render, new App::RenderEventHandler(TRT_MouseListener2D_MouseOverChecker, this));
}
