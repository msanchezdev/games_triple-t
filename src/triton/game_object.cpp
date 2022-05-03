#include <algorithm>
#include "game_object.hpp"
#include "utils.hpp"

void TRT_Component::OnAttach(TRT_GameObject* game_object) {
    verbose("OnAttach not implemented for this component: %p", this);
}

void TRT_Component::OnDetach(TRT_GameObject* game_object) {
    verbose("OnDetach not implemented for this component: %p", this);
}

void TRT_Component::AddToGameObject(TRT_GameObject* game_object) {
    game_object->AddComponent(this);
}

void TRT_Component::RemoveFromGameObject(TRT_GameObject* game_object) {
    game_object->RemoveComponent(this);
}

// ---------- GameObject ----------

TRT_GameObject::~TRT_GameObject() {
    for (auto& component : this->components) {
        RemoveComponent(component);
        delete component;
    }

    for (auto& event_data : this->events_data) {
        delete event_data.second;
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
