#pragma once

#include <vector>
#include "vector.hpp"
#include "component.hpp"

using namespace std;

namespace triton {
    class Transform : public Component {
    private:
        vector<Transform*> children;

    public:
        Transform* parent;
        Vector position;
        Vector scale;


        /**
         * Create a new transform
         */
        Transform(Transform* parent = nullptr);

        /**
         * Create a new transform with a given position
         */
        Transform(Vector position, Transform* parent = nullptr);

        /**
         * Create a new transform with a given position and scale
         */
        Transform(Vector position, Vector scale, Transform* parent = nullptr);
    };
}
