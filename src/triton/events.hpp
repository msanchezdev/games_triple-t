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

namespace triton {
    /**
     * @brief Contains details about the emitted event.
     */
    template <class SenderType, class ReferenceType, class DataType>
    struct EventArgs {
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
    using EventHandlerFunction = void (*)(EventArgs<SenderType, ReferenceType, DataType>* event);

    /**
     * Base Event Handler class.
     * To create a new event handler, use the DefineEventHandler macro which will
     * generate a new class inheriting from this one.
     *
     *     enum EnemyEventType {
     *        GunShot,
     *     }
     *     typedef struct {
     *        Player* gun;
     *        int damage;
     *     } GunShotEvent;
     *     DefineEventHandler(GunShotEventHandler, Enemy, GunShotEvent);
     *     EventListener<Enemy, EnemyEventType> enemy_events;
     *
     * Will generate a new class GunShotEventHandler which receives a handler function with the following signature
     * where ReferenceType will be automatically deduced when instantiating the `GunShotEventHandler` class:
     *
     *     void Handler(EventArgs<Enemy, **ReferenceType**, GunShotEvent> event);
     *
     * And is used to subscribe to the event type EVENT_GUN_SHOOT.
     *
     *     enemy_events->Subscribe(EnemyEventType::GunShot, new GunShotEventHandler(Handler, &enemy));
     */
    template <class SenderType, class ReferenceType, class DataType>
    class EventHandler {
    private:
        EventHandlerFunction<SenderType, ReferenceType, DataType> _handler;
        ReferenceType* reference;

    public:
        EventHandler(EventHandlerFunction<SenderType, ReferenceType, DataType> handler)
            : _handler(handler) {
        }
        EventHandler(EventHandlerFunction<SenderType, ReferenceType, DataType> handler, ReferenceType* reference)
            : _handler(handler), reference(reference) {
        }

        // call the handler function, reference value will be overwritten
        void call(EventArgs<SenderType, ReferenceType, DataType>* event) {
            event->reference = reference;
            this->_handler(event);
        }

        bool compare(EventHandler<SenderType, ReferenceType, DataType>* other) {
            return this->_handler == other->_handler;
        }
    };

    template <class SenderType, class EventType>
    class EventListener {
    private:
        SenderType* sender;

        // TODO: Make `ReferenceType` and `DataType` generic instead of void*
        map<EventType, vector<EventHandler<SenderType, nullptr_t, void>*>> handlers;

    public:
        EventListener(SenderType* sender) : sender(sender) {};
        ~EventListener();

        template<class ReferenceType, class DataType>
        void Subscribe(EventType type, EventHandler<SenderType, ReferenceType, DataType>* handler);

        template<class ReferenceType, class DataType>
        void Unsubscribe(EventType type, EventHandler<SenderType, ReferenceType, DataType>* handler);
        void Unsubscribe(EventType type);
        void Unsubscribe();

        template <class DataType>
        void Emit(EventType type, DataType* data);
    };

    template <class SenderType, class EventType>
    class WithEventListener {
    private:
        EventListener<SenderType, EventType> listener;

    public:
        WithEventListener(SenderType* sender) : listener(sender) {};

        template<class ReferenceType, class DataType>
        void Subscribe(EventType type, EventHandler<SenderType, ReferenceType, DataType>* handler) {
            listener.Subscribe(type, handler);
        }

        template<class ReferenceType, class DataType>
        void Unsubscribe(EventType type, EventHandler<SenderType, ReferenceType, DataType>* handler) {
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
    void EventListener<SenderType, EventType>::Emit(EventType type, DataType* data) {
        verbose("Emitting event %p (%s: %#x)", this->sender, typeid(EventType).name(), type);

        if (!this->handlers.count(type)) {
            verbose("No handlers for event %p (%s: %#x)", this->sender, typeid(EventType).name(), type);
            return;
        }

        // reference type is nullptr as the handlers are the only ones
        EventArgs<SenderType, nullptr_t, void> event;
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
    void EventListener<SenderType, EventType>::Subscribe(EventType type, EventHandler<SenderType, ReferenceType, DataType>* handler) {
        verbose("Subscribing %p (%s: %#x) = %p", this->sender, typeid(EventType).name(), type, handler);

        if (!this->handlers.count(type)) {
            // TODO: Make `ReferenceType` and `DataType` generic instead of void*
            this->handlers[type] = vector<EventHandler<SenderType, nullptr_t, void>*>();
        }

        this->handlers[type].push_back((EventHandler<SenderType, nullptr_t, void>*)handler);
    }

    template <class SenderType, class EventType>
    template<class ReferenceType, class DataType>
    void EventListener<SenderType, EventType>::Unsubscribe(EventType type, EventHandler<SenderType, ReferenceType, DataType>* handler) {
        verbose("Unsubscribing %p (%s: %#x) = %p", this->sender, typeid(EventType).name(), type, handler);

        if (!this->handlers.count(type)) {
            verbose("No handlers for event %p (%s: %#x)", this->sender, typeid(EventType).name(), type);
            return;
        }

        bool removed = false;
        for (auto it = this->handlers[type].begin(); it != this->handlers[type].end(); it++) {
            if ((*it)->compare((EventHandler<SenderType, nullptr_t, void>*)handler)) {
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
    EventListener<SenderType, EventType>::~EventListener() {
        for (auto event_type : this->handlers) {
            for (auto handler : event_type.second) {
                Unsubscribe(event_type.first, handler);
            }
        }
    }

    template <class SenderType, class EventType>
    void EventListener<SenderType, EventType>::Unsubscribe(EventType type) {
        for (auto handler : this->handlers[type]) {
            this->handlers[type].erase(handler);
        }
    }

    template <class SenderType, class EventType>
    void EventListener<SenderType, EventType>::Unsubscribe() {
        for (auto event_type : this->handlers) {
            for (auto handler : event_type.second) {
                Unsubscribe(event_type.first, handler);
            }
        }
    }
}

#define DefineEventHandler(name, sender_type, data_type) \
    template <class ReferenceType, class DataType = data_type> \
    class name : public EventHandler<sender_type, ReferenceType, DataType> { \
    public: \
        name(void (*handler)(EventArgs<sender_type, ReferenceType, DataType>* event)) \
            : EventHandler<sender_type, ReferenceType, DataType>(handler, nullptr) { \
        }; \
        name(void (*handler)(EventArgs<sender_type, ReferenceType, DataType>* event), ReferenceType* reference) \
            : EventHandler<sender_type, ReferenceType, DataType>(handler, reference) { \
        }; \
    };
