#include <algorithm>
#include <string>
#include "app.hpp"
#include "game_object.hpp"
#include "utils.hpp"

using namespace triton;

void Component::OnEnable(GameObject* game_object) {
    verbose("OnEnable not implemented for component %s", typeid(*this).name());
}

void Component::OnDisable(GameObject* game_object) {
    verbose("OnDisable not implemented for component %s", typeid(*this).name());
}

bool Component::IsEnabled() {
    return enabled;
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
    if (this->owner != nullptr && owner != nullptr) {
        warn("Component %s already has an owner", typeid(*this).name());
        return;
    }

    this->owner = owner;
}

void Component::Enable() {
    this->enabled = true;
    this->OnEnable(this->owner);
}

void Component::Disable() {
    this->enabled = false;
    this->OnDisable(this->owner);
}

// ---------- GameObject ----------

GameObject::GameObject(string name) : name(name) {};

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

    component->SetOwner(this);

    if (component->IsEnabled()) {
        component->OnEnable(this);
    }
}

void GameObject::RemoveComponent(Component* component) {
    remove_if(
        this->components.begin(),
        this->components.end(),
        [component](void* c) {
            return c == component;
        }
    );

    component->OnDisable(this);
    component->SetOwner(nullptr);
}
