#pragma once
#include <map>
#include <list>
#include "utils.hpp"
#include "event_handlers.hpp"

using namespace std;

template<class type_t, class... args_t>
struct deduce_first { using type = type_t; };

template<typename... args_t>
using first_t = typename deduce_first<args_t...>::type;

class TRT_EventListener {
public:
    ~TRT_EventListener();

    map<
        TRT_EventType,
        map<TRT_GameObject*, list<TRT_EventHandler<>*>>
    > handlers;

    template<class D>
    void Subscribe(TRT_GameObject* object, TRT_EventType type, TRT_EventHandler<D>* handler);

    template<class D>
    void Unsubscribe(TRT_GameObject* object, TRT_EventType type, TRT_EventHandler<D>* handler);

    void Unsubscribe(TRT_GameObject* object, TRT_EventType type);
    void Unsubscribe(TRT_GameObject* object);

    template<class ... Args>
    void Emit(TRT_GameObject* object, TRT_EventType type, Args ... args);
    void Emit(TRT_GameObject* object, TRT_EventType type);

    template<class ... Args>
    void Broadcast(TRT_EventType type, Args ... args);
    void Broadcast(TRT_EventType type);
};

template <class ... Args>
void TRT_EventListener::Emit(TRT_GameObject* object, TRT_EventType type, Args ... args) {
    verbose("Emitting event %p (TRT_EventType: %#x)", object, type);

    if (!this->handlers.count(type)) {
        verbose("No handlers for event (TRT_EventType: %#x)", type);
        return;
    }

    if (!this->handlers[type].count(object)) {
        verbose("No handlers for event %p (TRT_EventType: %#x)", object, type);
        return;
    }

    TRT_EventArgs<> event;
    event.sender = object;
    event.data = (TRT_EventArgs<>*)(object->GetEventData<first_t<Args...>>(type));

    for (auto handler : this->handlers[type][object]) {
        verbose("Calling handler %p (TRT_EventType: %#x) = %p", object, type, handler);
        handler->call(&event, args...);
    }
}

template <class ... Args>
void TRT_EventListener::Broadcast(TRT_EventType type, Args ... args) {
    verbose("Broadcasting event (TRT_EventType: %#x)", type);

    if (!this->handlers.count(type)) {
        verbose("No handlers for event (TRT_EventType: %#x)", type);
        return;
    }

    for (auto object_list : this->handlers[type]) {
        TRT_EventArgs<> event;
        event.sender = object_list.first;
        event.data = (TRT_EventArgs<>*)(object_list.first->GetEventData<first_t<Args...>>(type));

        for (auto handler : object_list.second) {
            verbose("Calling handler %p (TRT_EventType: %#x) = %p", object_list.first, type, handler);
            handler->call(&event, args...);
        }
    }
}


template<class D>
void TRT_EventListener::Subscribe(TRT_GameObject* object, TRT_EventType type, TRT_EventHandler<D>* handler) {
    verbose("Subscribing %p (TRT_EventType: %#x) = %p", object, type, handler);

    if (!this->handlers.count(type)) {
        this->handlers[type] = map<TRT_GameObject*, list<TRT_EventHandler<>*>>();
    }

    if (!this->handlers[type].count(object)) {
        this->handlers[type][object] = list<TRT_EventHandler<>*>();
    }

    this->handlers[type][object].push_back((TRT_EventHandler<>*)handler);
}

template<class D>
void TRT_EventListener::Unsubscribe(TRT_GameObject* object, TRT_EventType type, TRT_EventHandler<D>* handler) {
    verbose("Unsubscribing %p (TRT_EventType: %#x) = %p", object, type, handler);

    if (!this->handlers.count(type)) {
        return;
    }

    if (!this->handlers[type].count(object)) {
        return;
    }

    this->handlers[type][object].remove_if([handler](TRT_EventHandler<>* h) {
        return h->compare((TRT_EventHandler<>*)handler);
    });
}
