#include <string>
#include <list>
#include <vector>
#include <triton/utils.hpp>
#include <triton/types/component.hpp>
#include <triton/types/game_object.hpp>

using namespace triton;

GameObject::GameObject(string name)
    : name(name) {
}


GameObject* GameObject::SetName(string name) {
    this->name = name;
    return this;
}

string GameObject::GetName() {
    return this->name;
}

void GameObject::AddComponent(Component* component) {
    component->Attach(this);
    this->components.push_back(component);
}

void GameObject::RemoveComponent(Component* component) {
    component->Detach();
    for (auto it = this->components.begin(); it != this->components.end(); ++it) {
        if (*it == component) {
            this->components.erase(it);
            break;
        }
    }
}
