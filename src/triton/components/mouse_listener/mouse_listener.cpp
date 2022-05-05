#include <csignal>
#include <SDL2/SDL_timer.h>
#include <triton/app.hpp>
#include <triton/components/sprite/sprite.hpp>
#include "mouse_listener.hpp"

void TRT_MouseListener2D_MouseChecker(TRT_EventArgs<TRT_MouseMoveEventData>* object, TRT_MouseMoveEvent* event) {
    auto event_data = object->sender->GetEventData<TRT_MouseMoveEventData>(TRT_EventType::EVENT_MOUSE_MOVE);
    auto mouse_listeners = object->sender->GetComponents<TRT_MouseListener2D>();

    // FIXME: only checks against first sprite attached to the component
    TRT_Sprite* sprite = object->sender->GetComponent<TRT_Sprite>();
    SDL_Point mouse_pos = { event->x, event->y };
    SDL_Rect sprite_rect = { sprite->position->x, sprite->position->y, sprite->size->width, sprite->size->height };

    for (auto mouse_listener : mouse_listeners) {
        if (SDL_PointInRect(&mouse_pos, &sprite_rect)) {
            if (!event_data->is_inside) {
                event_data->is_inside = true;

                if (mouse_listener->handlers.on_mouse_enter) {
                    TRT_MouseEnterEvent* enter_event = event;
                    mouse_listener->handlers.on_mouse_enter->call((TRT_EventArgs<void>*)object, enter_event);
                }
            } else {
                if (mouse_listener->handlers.on_mouse_move) {
                    mouse_listener->handlers.on_mouse_move->call(object, event);
                }
            }
        } else if (event_data->is_inside) {
            event_data->is_inside = false;

            TRT_MouseLeaveEvent* leave_event = event;
            mouse_listener->handlers.on_mouse_leave->call((TRT_EventArgs<void>*)object, leave_event);
        }
    }
}

void TRT_MouseListener2D_MouseDownChecker(TRT_EventArgs<>* object, TRT_MouseDownEvent* event) {
    auto mouse_listeners = object->sender->GetComponents<TRT_MouseListener2D>();

    TRT_Sprite* sprite = object->sender->GetComponent<TRT_Sprite>();
    SDL_Point mouse_pos = { event->x, event->y };
    SDL_Rect sprite_rect = { sprite->position->x, sprite->position->y, sprite->size->width, sprite->size->height };
    for (auto mouse_listener : mouse_listeners) {
        if (SDL_PointInRect(&mouse_pos, &sprite_rect)) {
            if (mouse_listener->handlers.on_mouse_down) {
                mouse_listener->handlers.on_mouse_down->call((TRT_EventArgs<void>*)object, event);
            }
        }
    }
}

void TRT_MouseListener2D_MouseUpChecker(TRT_EventArgs<>* object, TRT_MouseUpEvent* event) {
    auto mouse_listeners = object->sender->GetComponents<TRT_MouseListener2D>();

    TRT_Sprite* sprite = object->sender->GetComponent<TRT_Sprite>();
    SDL_Point mouse_pos = { event->x, event->y };
    SDL_Rect sprite_rect = { sprite->position->x, sprite->position->y, sprite->size->width, sprite->size->height };
    for (auto mouse_listener : mouse_listeners) {
        if (SDL_PointInRect(&mouse_pos, &sprite_rect)) {
            if (mouse_listener->handlers.on_mouse_up) {
                mouse_listener->handlers.on_mouse_up->call((TRT_EventArgs<void>*)object, event);
            }
        }
    }
}

void TRT_MouseListener2D_MouseOverChecker(TRT_EventArgs<>* object, TRT_MouseOverEvent* event) {
    auto event_data = object->sender->GetEventData<TRT_MouseMoveEventData>(TRT_EventType::EVENT_MOUSE_MOVE);
    auto mouse_listeners = object->sender->GetComponents<TRT_MouseListener2D>();

    for (auto mouse_listener : mouse_listeners) {
        if (event_data->is_inside) {
            if (mouse_listener->handlers.on_mouse_over) {
                TRT_MouseOverEvent over_event;
                over_event.state = SDL_GetMouseState(&over_event.x, &over_event.y);
                over_event.timestamp = SDL_GetTicks();
                over_event.windowID = SDL_GetWindowID(app.window);
                mouse_listener->handlers.on_mouse_over->call((TRT_EventArgs<void>*)object, &over_event);
            }
        }
    }
}

void TRT_MouseListener2D::OnAttach(TRT_GameObject* game_object) {
    app.events->Subscribe(game_object, TRT_EventType::EVENT_MOUSE_MOVE, new TRT_MouseMoveHandler(TRT_MouseListener2D_MouseChecker));
    app.events->Subscribe(game_object, TRT_EventType::EVENT_MOUSE_DOWN, new TRT_MouseDownHandler(TRT_MouseListener2D_MouseDownChecker));
    app.events->Subscribe(game_object, TRT_EventType::EVENT_MOUSE_UP, new TRT_MouseUpHandler(TRT_MouseListener2D_MouseUpChecker));
    app.events->Subscribe(game_object, TRT_EventType::EVENT_RENDER, new TRT_MouseOverHandler(TRT_MouseListener2D_MouseOverChecker));
}

void TRT_MouseListener2D::OnDetach(TRT_GameObject* game_object) {
    app.events->Unsubscribe(game_object, TRT_EventType::EVENT_MOUSE_MOVE, new TRT_MouseMoveHandler(TRT_MouseListener2D_MouseChecker));
    app.events->Unsubscribe(game_object, TRT_EventType::EVENT_MOUSE_DOWN, new TRT_MouseDownHandler(TRT_MouseListener2D_MouseDownChecker));
    app.events->Unsubscribe(game_object, TRT_EventType::EVENT_MOUSE_UP, new TRT_MouseUpHandler(TRT_MouseListener2D_MouseUpChecker));
    app.events->Unsubscribe(game_object, TRT_EventType::EVENT_RENDER, new TRT_MouseOverHandler(TRT_MouseListener2D_MouseOverChecker));
}
