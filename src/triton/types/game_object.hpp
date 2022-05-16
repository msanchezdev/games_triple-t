#pragma once

#include <typeinfo>
#include <string>
#include <list>
#include <vector>
#include <triton/types/component.hpp>
#include <triton/types/transform.hpp>

using namespace std;

namespace triton {
    class GameObject {
    private:
        Transform transform;
        list<Component*> components;
        string name;

    public:

        GameObject() : name("GameObject") {};
        GameObject(string name);

        GameObject* SetName(string name);
        string GetName();

        template <class T>
        const vector<T*> GetComponents();

        template <class T>
        T* GetComponent();

        void AddComponent(Component* component);
        void RemoveComponent(Component* component);
    };

    template <class T>
    const vector<T*> GameObject::GetComponents() {
        vector<T*> components;

        for (auto component : this->components) {
            if (typeid(T) == typeid(*component)) {
                components.push_back((T*)component);
            }
        }

        return components;
    }

    template <class T>
    T* GameObject::GetComponent() {
        for (auto component : this->components) {
            if (typeid(T) == typeid(*component)) {
                return (T*)component;
            }
        }

        return nullptr;
    }
}
