#include "vector.hpp"
#include "transform.hpp"

using namespace triton;

Transform::Transform(Transform* parent)
    : Transform(Vector(0, 0), parent) {
}

Transform::Transform(Vector position, Transform* parent)
    : Transform(position, Vector(1, 1), parent) {
}

Transform::Transform(Vector position, Vector scale, Transform* parent)
    : parent(parent), position(position), scale(scale) {
    if (parent != nullptr) {
        parent->children.push_back(this);
    }
}
