#pragma once

#include <map>
#include <vector>
#include <typeinfo>

class TRT_GameObject;

#include "event_handlers.hpp"

using namespace std;

class TRT_Component {
public:
    void AddToGameObject(TRT_GameObject* game_object);
    void RemoveFromGameObject(TRT_GameObject* game_object);

    virtual void OnAttach(TRT_GameObject* game_object);
    virtual void OnDetach(TRT_GameObject* game_object);
};

class TRT_GameObject {
private:
    map<TRT_EventType, void*> events_data;
    vector<TRT_Component*> components;

public:
    ~TRT_GameObject();

    template <class T = char>
    T* GetEventData(TRT_EventType type) {
        if (!this->events_data.count(type)) {
            this->events_data[type] = new T();
        }

        return (T*)this->events_data[type];
    }

    // function to get all components of a given type
    template <class T>
    vector<T*> GetComponents() {
        vector<T*> components;
        for (auto component : this->components) {
            if (typeid(T) == typeid(*component)) {
                components.push_back((T*)component);
            }
        }
        return components;
    }

    // get first component of a given type
    template <class T>
    T* GetComponent() {
        for (auto component : this->components) {
            if (typeid(T) == typeid(*component)) {
                return (T*)component;
            }
        }

        return nullptr;
    }

    void AddComponent(TRT_Component* component);
    void RemoveComponent(TRT_Component* component);
};
