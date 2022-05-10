// TODO: Add data to events
#include <csignal>
#include <SDL2/SDL_timer.h>
#include <triton/app.hpp>
#include <triton/components/sprite/sprite.hpp>
#include "mouse_listener.hpp"

using namespace triton;

void MouseListener_MouseChecker(EventArgs<App, MouseListener, App::MouseMoveEvent>* event) {
    auto listener = event->reference;
    SDL_Point mouse_pos = { event->data->x, event->data->y };
    SDL_Rect listener_rect = (SDL_Rect)*listener->check_rect;

    if (SDL_PointInRect(&mouse_pos, &listener_rect)) {
        if (!listener->is_inside) {
            listener->is_inside = true;

            MouseListener::MouseEnterEvent enter_event;
            listener->Emit(MouseListener::EventType::MouseEnter, &enter_event);
        } else {
            MouseListener::MouseMoveEvent move_event;
            listener->Emit(MouseListener::EventType::MouseMove, &move_event);
        }
    } else if (listener->is_inside) {
        listener->is_inside = false;

        MouseListener::MouseLeaveEvent leave_event;
        listener->Emit(MouseListener::EventType::MouseLeave, &leave_event);
    }
}

void MouseListener_MouseDownChecker(EventArgs<App, MouseListener, App::MouseDownEvent>* event) {
    auto listener = event->reference;
    SDL_Point mouse_pos = { event->data->x, event->data->y };
    SDL_Rect listener_rect = (SDL_Rect)*listener->check_rect;

    if (SDL_PointInRect(&mouse_pos, &listener_rect)) {
        MouseListener::MouseDownEvent down_event;
        listener->Emit(MouseListener::EventType::MouseDown, &down_event);
    }
}

void MouseListener_MouseUpChecker(EventArgs<App, MouseListener, App::MouseUpEvent>* event) {
    auto listener = event->reference;
    SDL_Point mouse_pos = { event->data->x, event->data->y };
    SDL_Rect listener_rect = (SDL_Rect)*listener->check_rect;

    if (SDL_PointInRect(&mouse_pos, &listener_rect)) {
        MouseListener::MouseEnterEvent up_event;
        listener->Emit(MouseListener::EventType::MouseUp, &up_event);
    }
}

void MouseListener_MouseOverChecker(EventArgs<App, MouseListener, App::RenderEvent>* event) {
    auto listener = event->reference;

    if (listener->is_inside) {
        MouseListener::MouseOverEvent over_event;
        // over_event.state = SDL_GetMouseState(&over_event.x, &over_event.y);
        // over_event.timestamp = SDL_GetTicks();
        // over_event.windowID = SDL_GetWindowID(app.window);
        listener->Emit(MouseListener::EventType::MouseOver, &over_event);
    }
}

void MouseListener::OnAttach(GameObject* game_object) {
    app.events.Subscribe(App::EventType::MouseMove, new App::MouseMoveEventHandler(MouseListener_MouseChecker, this));
    app.events.Subscribe(App::EventType::MouseButtonDown, new App::MouseDownEventHandler(MouseListener_MouseDownChecker, this));
    app.events.Subscribe(App::EventType::MouseButtonUp, new App::MouseUpEventHandler(MouseListener_MouseUpChecker, this));
    app.events.Subscribe(App::EventType::Render, new App::RenderEventHandler(MouseListener_MouseOverChecker, this));
}

void MouseListener::OnDetach(GameObject* game_object) {
    app.events.Unsubscribe(App::EventType::MouseMove, new App::MouseMoveEventHandler(MouseListener_MouseChecker, this));
    app.events.Unsubscribe(App::EventType::MouseButtonDown, new App::MouseDownEventHandler(MouseListener_MouseDownChecker, this));
    app.events.Unsubscribe(App::EventType::MouseButtonUp, new App::MouseUpEventHandler(MouseListener_MouseUpChecker, this));
    app.events.Unsubscribe(App::EventType::Render, new App::RenderEventHandler(MouseListener_MouseOverChecker, this));
}
