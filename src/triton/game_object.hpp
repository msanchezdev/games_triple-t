#pragma once

#include <map>
#include <string>
#include <vector>
#include <typeinfo>

class TRT_GameObject;

#include "events.hpp"

using namespace std;

class TRT_Component {
public:
    TRT_GameObject* owner;
    void AddToGameObject(TRT_GameObject* game_object);
    void RemoveFromGameObject(TRT_GameObject* game_object);

    TRT_GameObject* GetOwner();
    void SetOwner(TRT_GameObject* owner);

    virtual void OnAttach(TRT_GameObject* game_object);
    virtual void OnDetach(TRT_GameObject* game_object);
};

class TRT_GameObject {
private:
    vector<TRT_Component*> components;

public:
    string name;
    TRT_GameObject() : name("GameObject") {};
    TRT_GameObject(string name);
    ~TRT_GameObject();

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
