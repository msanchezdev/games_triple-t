/**
 * Events are used to notify other objects of changes in the application.
 *
 * An object can subscribe to an event on the other object's event listener.
 * MySubject subject;
 * subject.events.Subscribe(SubjectEventType::EventA, new EventHandler(Handler, ReferenceData));
 *
 * The event handler's signature is:
 * void Handler(EventArgs<MySubject, ReferenceData, EventData> event);
 *
 * The object will then emit the event (notify all subscribers)
 * subject.events.Emit(SubjectEventType::EventA, new EventData());
 */

#pragma once
#include <map>
#include <vector>
#include <typeinfo>
#include "utils.hpp"

using namespace std;

/**
 * @brief Contains details about the emitted event.
 */
template <class SenderType, class ReferenceType, class DataType>
struct TRT_EventArgs {
    // The object emitting the event (the one with the event listener attached)
    SenderType* sender;

    // Arbitrary data that can be used by the event handler
    ReferenceType* reference;

    // Data about the event, specific to the event type
    DataType* data;
};

/**
 * @brief Function for handling events
 */
template <class SenderType, class ReferenceType, class DataType>
using TRT_EventHandlerFunction = void (*)(TRT_EventArgs<SenderType, ReferenceType, DataType>* event);

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
template <class SenderType, class ReferenceType, class DataType>
class TRT_EventHandler {
private:
    TRT_EventHandlerFunction<SenderType, ReferenceType, DataType> _handler;
    ReferenceType* reference;

public:
    TRT_EventHandler(TRT_EventHandlerFunction<SenderType, ReferenceType, DataType> handler)
        : _handler(handler) {
    }
    TRT_EventHandler(TRT_EventHandlerFunction<SenderType, ReferenceType, DataType> handler, ReferenceType* reference)
        : _handler(handler), reference(reference) {
    }

    // call the handler function, reference value will be overwritten
    void call(TRT_EventArgs<SenderType, ReferenceType, DataType>* event) {
        event->reference = reference;
        this->_handler(event);
    }

    bool compare(TRT_EventHandler<SenderType, ReferenceType, DataType>* other) {
        return this->_handler == other->_handler;
    }
};

template <class SenderType, class EventType>
class TRT_EventListener {
private:
    SenderType* sender;

    // TODO: Make `ReferenceType` and `DataType` generic instead of void*
    map<EventType, vector<TRT_EventHandler<SenderType, nullptr_t, void>*>> handlers;

public:
    TRT_EventListener(SenderType* sender) : sender(sender) {};
    ~TRT_EventListener();

    template<class ReferenceType, class DataType>
    void Subscribe(EventType type, TRT_EventHandler<SenderType, ReferenceType, DataType>* handler);

    template<class ReferenceType, class DataType>
    void Unsubscribe(EventType type, TRT_EventHandler<SenderType, ReferenceType, DataType>* handler);
    void Unsubscribe(EventType type);
    void Unsubscribe();

    template <class DataType>
    void Emit(EventType type, DataType* data);
};

template <class SenderType, class EventType>
class TRT_WithEventListener {
private:
    TRT_EventListener<SenderType, EventType> listener;

public:
    TRT_WithEventListener(SenderType* sender) : listener(sender) {};

    template<class ReferenceType, class DataType>
    void Subscribe(EventType type, TRT_EventHandler<SenderType, ReferenceType, DataType>* handler) {
        listener.Subscribe(type, handler);
    }

    template<class ReferenceType, class DataType>
    void Unsubscribe(EventType type, TRT_EventHandler<SenderType, ReferenceType, DataType>* handler) {
        listener.Unsubscribe(type, handler);
    }

    void Unsubscribe(EventType type) {
        listener.Unsubscribe(type);
    }

    void Unsubscribe() {
        listener.Unsubscribe();
    }

    template <class DataType>
    void Emit(EventType type, DataType* data) {
        listener.Emit(type, data);
    }
};

template <class SenderType, class EventType>
template <class DataType>
void TRT_EventListener<SenderType, EventType>::Emit(EventType type, DataType* data) {
    verbose("Emitting event %p (%s: %#x)", this->sender, typeid(EventType).name(), type);

    if (!this->handlers.count(type)) {
        verbose("No handlers for event %p (%s: %#x)", this->sender, typeid(EventType).name(), type);
        return;
    }

    // reference type is nullptr as the handlers are the only ones
    TRT_EventArgs<SenderType, nullptr_t, void> event;
    event.sender = this->sender;
    event.data = (void*)data;
    event.reference = nullptr; // will be overwritten by each handler

    for (auto handler : this->handlers[type]) {
        verbose("Calling handler %p (%s: %#x) = %p", this->sender, typeid(EventType).name(), type, handler);
        handler->call(&event);
    }
}

template <class SenderType, class EventType>
template<class ReferenceType, class DataType>
void TRT_EventListener<SenderType, EventType>::Subscribe(EventType type, TRT_EventHandler<SenderType, ReferenceType, DataType>* handler) {
    verbose("Subscribing %p (%s: %#x) = %p", this->sender, typeid(EventType).name(), type, handler);

    if (!this->handlers.count(type)) {
        // TODO: Make `ReferenceType` and `DataType` generic instead of void*
        this->handlers[type] = vector<TRT_EventHandler<SenderType, nullptr_t, void>*>();
    }

    this->handlers[type].push_back((TRT_EventHandler<SenderType, nullptr_t, void>*)handler);
}

template <class SenderType, class EventType>
template<class ReferenceType, class DataType>
void TRT_EventListener<SenderType, EventType>::Unsubscribe(EventType type, TRT_EventHandler<SenderType, ReferenceType, DataType>* handler) {
    verbose("Unsubscribing %p (%s: %#x) = %p", this->sender, typeid(EventType).name(), type, handler);

    if (!this->handlers.count(type)) {
        verbose("No handlers for event %p (%s: %#x)", this->sender, typeid(EventType).name(), type);
        return;
    }

    bool removed = false;
    for (auto it = this->handlers[type].begin(); it != this->handlers[type].end(); it++) {
        if ((*it)->compare((TRT_EventHandler<SenderType, nullptr_t, void>*)handler)) {
            removed = true;
            this->handlers[type].erase(it);
            break;
        }
    }

    if (!removed) {
        verbose("%p is not a handler for event %p (%s: %#x)", handler, this->sender, typeid(EventType).name(), type);
    }
}

template <class SenderType, class EventType>
TRT_EventListener<SenderType, EventType>::~TRT_EventListener() {
    for (auto event_type : this->handlers) {
        for (auto handler : event_type.second) {
            Unsubscribe(event_type.first, handler);
        }
    }
}

template <class SenderType, class EventType>
void TRT_EventListener<SenderType, EventType>::Unsubscribe(EventType type) {
    for (auto handler : this->handlers[type]) {
        this->handlers[type].erase(handler);
    }
}

template <class SenderType, class EventType>
void TRT_EventListener<SenderType, EventType>::Unsubscribe() {
    for (auto event_type : this->handlers) {
        for (auto handler : event_type.second) {
            Unsubscribe(event_type.first, handler);
        }
    }
}

#define DefineEventHandler(name, sender_type, data_type) \
    template <class ReferenceType, class DataType = data_type> \
    class name : public TRT_EventHandler<sender_type, ReferenceType, DataType> { \
    public: \
        name(void (*handler)(TRT_EventArgs<sender_type, ReferenceType, DataType>* event), ReferenceType* reference) \
            : TRT_EventHandler<sender_type, ReferenceType, DataType>(handler, reference) { \
        }; \
    };
