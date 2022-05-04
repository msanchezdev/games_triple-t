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

void TRT_EventListener::Emit(TRT_GameObject* object, TRT_EventType type) {
    this->Emit(object, type, nullptr);
}

void TRT_EventListener::Broadcast(TRT_EventType type) {
    this->Broadcast(type, nullptr);
}
