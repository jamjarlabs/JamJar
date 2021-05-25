#include "geometry/vector_2d.hpp"

JamJar::Vector2D::Vector2D() : Vector2D(0, 0) {}

JamJar::Vector2D::Vector2D(float x, float y) : x(x), y(y) {}

JamJar::Vector2D JamJar::Vector2D::operator+(const Vector2D &other) const {
    return Vector2D(this->x + other.x, this->y + other.y);
}

void JamJar::Vector2D::operator+=(const Vector2D &other) {
    this->x += other.x;
    this->y += other.y;
}

JamJar::Vector2D JamJar::Vector2D::operator-(const Vector2D &other) const {
    return Vector2D(this->x - other.x, this->y - other.y);
}

void JamJar::Vector2D::operator-=(const Vector2D &other) {
    this->x -= other.x;
    this->y -= other.y;
}

JamJar::Vector2D JamJar::Vector2D::operator*(const Vector2D &other) const {
    return Vector2D(this->x * other.x, this->y * other.y);
}

void JamJar::Vector2D::operator*=(const Vector2D &other) {
    this->x *= other.x;
    this->y *= other.y;
}

JamJar::Vector2D JamJar::Vector2D::operator*(float scaler) const {
    return Vector2D(this->x * scaler, this->y * scaler);
}
