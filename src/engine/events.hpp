#pragma once
#include <map>
#include <list>
#include "utils.hpp"
#include "event_handlers.hpp"

using namespace std;

class TRT_EventListener {
public:
    ~TRT_EventListener();

    map<
        TRT_EventType,
        map<TRT_EventSubject, list<TRT_EventHandler*>>
    > handlers;

    void Subscribe(TRT_EventSubject object, TRT_EventType type, TRT_EventHandler* handler);
    void Unsubscribe(TRT_EventSubject object, TRT_EventType type, TRT_EventHandler* handler);
    void Unsubscribe(TRT_EventSubject object, TRT_EventType type);
    void Unsubscribe(TRT_EventSubject object);

    template<class ... Args>
    void Emit(TRT_EventSubject object, TRT_EventType type, Args ... args);

    template<class ... Args>
    void Broadcast(TRT_EventType type, Args ... args);
};

template <class ... Args>
void TRT_EventListener::Emit(TRT_EventSubject object, TRT_EventType type, Args ... args) {
    verbose("Emitting event %p (TRT_EventType: %x)", object, type);

    if (!this->handlers.count(type)) {
        verbose("No handlers for event (TRT_EventType: %x)", type);
        return;
    }

    if (!this->handlers[type].count(object)) {
        verbose("No handlers for event %p (TRT_EventType: %x)", object, type);
        return;
    }

    for (auto handler : this->handlers[type][object]) {
        verbose("Calling handler %p (TRT_EventType: %x) = %p", object, type, handler);
        handler->call(object, args...);
    }
}

template <class ... Args>
void TRT_EventListener::Broadcast(TRT_EventType type, Args ... args) {
    verbose("Broadcasting event (TRT_EventType: %x)", type);

    if (!this->handlers.count(type)) {
        verbose("No handlers for event (TRT_EventType: %x)", type);
        return;
    }

    for (auto object_list : this->handlers[type]) {
        for (auto handler : object_list.second) {
            verbose("Calling handler %p (TRT_EventType: %x) = %p", object_list.first, type, handler);
            handler->call(object_list.first, args...);
        }
    }
}
