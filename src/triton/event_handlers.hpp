#pragma once

enum TRT_EventType {
    EVENT_RENDER,
    EVENT_MOUSE_MOVE,
    EVENT_MOUSE_DOWN,
    EVENT_MOUSE_UP,

    EVENT_TOTAL,
};

#include <SDL2/SDL_events.h>
#include "game_object.hpp"

template <class D = void>
struct TRT_EventArgs {
    TRT_GameObject* sender;

    // private data used by the event handler associated to the game object
    D* data;
};

template<class D = void>
using TRT_EventHandlerFunction = void (*)(TRT_EventArgs<D>*, ...);

/**
 * Base Event Handler class.
 * To create a new event handler, use the DefineEventHandler macro which will
 * generate a new class inheriting from this one.
 *
 * Example:
 * DefineEventHandler(TRT_OnGunShoot, TRT_EventSubject object, int damage);
 *
 * Will generate a new class TRT_OnGunShoot which receives a handler function with the following signature:
 * void Handler(TRT_EventSubject object, int damage);
 *
 * And is used to subscribe to the event type EVENT_GUN_SHOOT.
 * event_listener->Subscribe(EVENT_GUN_SHOOT, object, new TRT_EventSubject(Handler));
 *
 * Instances of the generated class don't need to be disposed, they are automatically disposed when the event is
 * unsubscribed.
 */
template<class D = void>
class TRT_EventHandler {
private:
    TRT_EventHandlerFunction<D> _handler;

public:
    TRT_EventHandler(TRT_EventHandlerFunction<D> handler) {
        this->_handler = handler;
    }

    template <class ... Args>
    void call(TRT_EventArgs<D>* object, Args ... args) {
        this->_handler(object, args...);
    }

    bool compare(TRT_EventHandler<>* other) {
        return this->_handler == other->_handler;
    }
};

#define DefineEventHandlerWithoutArgs(name, data_type) \
    class name : public TRT_EventHandler<data_type> { \
    public: \
        name(void (*handler)(TRT_EventArgs<data_type>* event)) \
            : TRT_EventHandler((TRT_EventHandlerFunction<data_type>)handler) { \
        }; \
    };

#define DefineEventHandler(name, data_type, ...) \
    class name : public TRT_EventHandler<data_type> { \
    public: \
        name(void (*handler)(TRT_EventArgs<data_type>* event, __VA_ARGS__)) \
            : TRT_EventHandler((TRT_EventHandlerFunction<data_type>)handler) { \
        }; \
    };

#pragma region Event Handler Classes
// all event handlers will receive the event subject as the first argument

DefineEventHandlerWithoutArgs(TRT_RenderEventHandler, void);

using TRT_MouseEnterEvent = SDL_MouseMotionEvent;
DefineEventHandler(TRT_MouseEnterHandler, void, TRT_MouseEnterEvent*);

using TRT_MouseLeaveEvent = SDL_MouseMotionEvent;
DefineEventHandler(TRT_MouseLeaveHandler, void, TRT_MouseLeaveEvent*);

using TRT_MouseUpEvent = SDL_MouseButtonEvent;
DefineEventHandler(TRT_MouseUpHandler, void, TRT_MouseUpEvent*);

using TRT_MouseDownEvent = SDL_MouseButtonEvent;
DefineEventHandler(TRT_MouseDownHandler, void, TRT_MouseDownEvent*);

typedef struct TRT_MouseMoveEventData {
    bool is_inside;
};
using TRT_MouseMoveEvent = SDL_MouseMotionEvent;
DefineEventHandler(TRT_MouseMoveHandler, TRT_MouseMoveEventData, TRT_MouseMoveEvent*);

struct TRT_MouseOverEvent {
    Uint32 timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
    Uint32 windowID;    /**< The window with mouse focus, if any */
    Uint32 state;       /**< The current button state */
    Sint32 x;           /**< X coordinate, relative to window */
    Sint32 y;           /**< Y coordinate, relative to window */
};
DefineEventHandler(TRT_MouseOverHandler, void, TRT_MouseOverEvent*);

#pragma endregion Event Handler Classes
