#include "utils.hpp"
#include "events.hpp"

TRT_EventListener::~TRT_EventListener() {
    for (auto type_map : this->handlers) {
        for (auto object_list : type_map.second) {
            for (auto handler : object_list.second) {
                Unsubscribe(object_list.first, type_map.first, handler);
            }
        }
    }
}

void TRT_EventListener::Subscribe(TRT_GameObject* object, TRT_EventType type, TRT_EventHandler<>* handler) {
    verbose("Subscribing %p (TRT_EventType: %#x) = %p", object, type, handler);

    if (!this->handlers.count(type)) {
        this->handlers[type] = map<TRT_GameObject*, list<TRT_EventHandler<>*>>();
    }

    if (!this->handlers[type].count(object)) {
        this->handlers[type][object] = list<TRT_EventHandler<>*>();
    }

    this->handlers[type][object].push_back(handler);
}

void TRT_EventListener::Unsubscribe(TRT_GameObject* object, TRT_EventType type, TRT_EventHandler<>* handler) {
    verbose("Unsubscribing %p (TRT_EventType: %#x) = %p", object, type, handler);

    if (!this->handlers.count(type)) {
        return;
    }

    if (!this->handlers[type].count(object)) {
        return;
    }

    this->handlers[type][object].remove_if([handler](TRT_EventHandler<>* h) {
        return h->compare(handler);
    });
}


template <>
void TRT_EventListener::Emit(TRT_GameObject* object, TRT_EventType type) {
    this->Emit(object, type, nullptr);
}

template <>
void TRT_EventListener::Broadcast(TRT_EventType type) {
    this->Broadcast(type, nullptr);
}
