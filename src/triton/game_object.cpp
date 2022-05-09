#include <algorithm>
#include <string>
#include "app.hpp"
#include "game_object.hpp"
#include "utils.hpp"

using namespace triton;

void Component::OnAttach(GameObject* game_object) {
    verbose("OnAttach not implemented for component %s", typeid(*this).name());
}

void Component::OnDetach(GameObject* game_object) {
    verbose("OnDetach not implemented for component %s", typeid(*this).name());
}

void Component::AddToGameObject(GameObject* game_object) {
    game_object->AddComponent(this);
}

void Component::RemoveFromGameObject(GameObject* game_object) {
    game_object->RemoveComponent(this);
}

GameObject* Component::GetOwner() {
    return owner;
}

void Component::SetOwner(GameObject* owner) {
    if (this->owner != nullptr) {
        this->owner->RemoveComponent(this);
    }

    this->owner = owner;
}

// ---------- GameObject ----------

/**
 * @brief Automatically registers the game object with the given name.
 */
GameObject::GameObject(string name) : name(name) {
    app.AddGameObject(this);
};

/**
 * @brief When the game object is removed from the app, it will be
 *       automatically destroyed.
 */
GameObject::~GameObject() {
    app.events.Unsubscribe();

    for (auto& component : this->components) {
        RemoveComponent(component);
    }

    verbose("GameObject %p destroyed", this);
}

void GameObject::AddComponent(Component* component) {
    this->components.push_back(component);
    component->OnAttach(this);
}

void GameObject::RemoveComponent(Component* component) {
    remove_if(
        this->components.begin(),
        this->components.end(),
        [component](void* c) {
            return c == component;
        }
    );
    component->OnDetach(this);
}
