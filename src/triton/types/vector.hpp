#pragma once

#include <SDL2/SDL_rect.h>

namespace triton {
    typedef struct Vector {
        int x;
        int y;

        Vector() : x(0), y(0) {}

        Vector(int x, int y) : x(x), y(y) {}

        operator SDL_Point() {
            return SDL_Point { x, y };
        }

        Vector operator+(Vector other) {
            return Vector(x + other.x, y + other.y);
        }

        Vector operator-(Vector other) {
            return Vector(x - other.x, y - other.y);
        }

        Vector operator*(float scalar) {
            return Vector(x * scalar, y * scalar);
        }

        Vector operator/(float scalar) {
            return Vector(x / scalar, y / scalar);
        }

        Vector operator-() {
            return Vector(-x, -y);
        }

        Vector& operator+=(Vector other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector& operator-=(Vector other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vector& operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        Vector& operator/=(float scalar) {
            x /= scalar;
            y /= scalar;
            return *this;
        }


        bool operator==(Vector other) {
            return x == other.x && y == other.y;
        }

        bool operator!=(Vector other) {
            return !(*this == other);
        }

        bool operator<(Vector other) {
            return x < other.x&& y < other.y;
        }

        bool operator>(Vector other) {
            return x > other.x && y > other.y;
        }

        bool operator<=(Vector other) {
            return x <= other.x && y <= other.y;
        }

        bool operator>=(Vector other) {
            return x >= other.x && y >= other.y;
        }
    } Vector;
}
