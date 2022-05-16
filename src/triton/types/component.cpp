#include <triton/utils.hpp>
#include <triton/types/game_object.hpp>
#include "component.hpp"

using namespace triton;

void Component::Attach(GameObject* game_object) {
    this->owner = game_object;
    this->OnAttach();

    if (this->enabled) {
        this->OnEnable();
    }
}

void Component::Detach() {
    if (this->enabled) {
        this->OnDisable();
    }

    this->OnDetach();
    this->owner = nullptr;
}

void Component::OnAttach() {
}

void Component::OnDetach() {
}

GameObject* Component::GetOwner() {
    return this->owner;
}

void Component::Enable() {
    this->enabled = true;

    if (this->owner) {
        this->OnEnable();
    }
}

void Component::Disable() {
    this->enabled = false;

    if (this->owner) {
        this->OnDisable();
    }
}

void Component::OnEnable() {
}

void Component::OnDisable() {
}

bool Component::IsEnabled() {
    return this->enabled;
}
