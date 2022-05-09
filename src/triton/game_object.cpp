#include <algorithm>
#include <string>
#include "app.hpp"
#include "game_object.hpp"
#include "utils.hpp"

void TRT_Component::OnAttach(TRT_GameObject* game_object) {
    verbose("OnAttach not implemented for component %s", typeid(*this).name());
}

void TRT_Component::OnDetach(TRT_GameObject* game_object) {
    verbose("OnDetach not implemented for component %s", typeid(*this).name());
}

void TRT_Component::AddToGameObject(TRT_GameObject* game_object) {
    game_object->AddComponent(this);
}

void TRT_Component::RemoveFromGameObject(TRT_GameObject* game_object) {
    game_object->RemoveComponent(this);
}

TRT_GameObject* TRT_Component::GetOwner() {
    return owner;
}

void TRT_Component::SetOwner(TRT_GameObject* owner) {
    if (this->owner != nullptr) {
        this->owner->RemoveComponent(this);
    }

    this->owner = owner;
}

// ---------- GameObject ----------

/**
 * @brief Automatically registers the game object with the given name.
 */
TRT_GameObject::TRT_GameObject(string name) : name(name) {
    app.AddGameObject(this);
};

/**
 * @brief When the game object is removed from the app, it will be
 *       automatically destroyed.
 */
TRT_GameObject::~TRT_GameObject() {
    app.events.Unsubscribe();

    for (auto& component : this->components) {
        RemoveComponent(component);
    }

    verbose("TRT_GameObject %p destroyed", this);
}

void TRT_GameObject::AddComponent(TRT_Component* component) {
    this->components.push_back(component);
    component->OnAttach(this);
}

void TRT_GameObject::RemoveComponent(TRT_Component* component) {
    remove_if(
        this->components.begin(),
        this->components.end(),
        [component](void* c) {
            return c == component;
        }
    );
    component->OnDetach(this);
}
