enum TRT_EventType {
    EVENT_RENDER,
    EVENT_MOUSE_MOVE,

    EVENT_TOTAL,
};

typedef void* TRT_EventSubject;

typedef void (*TRT_EventHandlerFunction)(TRT_EventSubject, ...);

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
class TRT_EventHandler {
private:
    TRT_EventHandlerFunction _handler;

public:
    TRT_EventHandler(TRT_EventHandlerFunction handler) {
        this->_handler = handler;
    }

    template <class ... Args>
    void call(TRT_EventSubject object, Args ... args) {
        this->_handler(object, args...);
    }

    bool compare(TRT_EventHandler* other) {
        return this->_handler == other->_handler;
    }
};

#define DefineEventHandler(name, ...) \
    class name : public TRT_EventHandler { \
    public: \
        name(void (*handler)(__VA_ARGS__)) \
            : TRT_EventHandler((TRT_EventHandlerFunction)handler) { \
        }; \
    };

#pragma region Event Handler Classes
// all event handlers will receive the event subject as the first argument

DefineEventHandler(TRT_RenderEventHandler, TRT_EventSubject object);

#pragma endregion Event Handler Classes
