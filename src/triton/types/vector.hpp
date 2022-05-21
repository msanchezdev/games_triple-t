#pragma once

#include <string>
#include <SDL2/SDL_rect.h>

namespace triton {
    typedef struct Vector {
        float x;
        float y;
        float z;

        Vector() : x(0), y(0), z(0) {}
        Vector(float x, float y) : x(x), y(y), z(0) {}

        Vector(float x, float y, float z) : x(x), y(y), z(z) {}

        Vector operator+(Vector other) {
            return Vector(x + other.x, y + other.y, z + other.z);
        }

        Vector operator-(Vector other) {
            return Vector(x - other.x, y - other.y, z - other.z);
        }

        Vector operator*(float scalar) {
            return Vector(x * scalar, y * scalar, z * scalar);
        }

        Vector operator/(float scalar) {
            return Vector(x / scalar, y / scalar, z / scalar);
        }

        Vector operator-() {
            return Vector(-x, -y, -z);
        }

        Vector& operator+=(Vector other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vector& operator-=(Vector other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        Vector& operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        Vector& operator/=(float scalar) {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }


        bool operator==(Vector other) {
            return x == other.x && y == other.y && z == other.z;
        }

        bool operator!=(Vector other) {
            return !(*this == other);
        }

        bool operator<(Vector other) {
            return (x < other.x) && y < other.y&& z < other.z;
        }

        bool operator>(Vector other) {
            return x > other.x && y > other.y && z > other.z;
        }

        bool operator<=(Vector other) {
            return x <= other.x && y <= other.y && z <= other.z;
        }

        bool operator>=(Vector other) {
            return x >= other.x && y >= other.y && z >= other.z;
        }

        std::string ToString() {
            return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
        }
    } Vector;
}
