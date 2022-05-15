#pragma once

#include <map>
#include <string>
#include <vector>
#include <typeinfo>
#include "events.hpp"

using namespace std;

namespace triton {
    class GameObject;

    class Component {
    private:
        bool enabled = true;

        void SetOwner(GameObject* owner);

        virtual void OnEnable(GameObject* game_object);
        virtual void OnDisable(GameObject* game_object);

        friend class GameObject;
    public:
        GameObject* owner;
        void AddToGameObject(GameObject* game_object);
        void RemoveFromGameObject(GameObject* game_object);

        GameObject* GetOwner();

        bool IsEnabled();
        void Enable();
        void Disable();

    };

    class GameObject {
    private:
        vector<Component*> components;

    public:
        string name;
        GameObject() : name("GameObject") {};
        GameObject(string name);
        ~GameObject();

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

        void AddComponent(Component* component);
        void RemoveComponent(Component* component);
    };
}
