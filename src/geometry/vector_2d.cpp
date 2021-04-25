#include "geometry/vector_2d.hpp"

JamJar::Vector2D::Vector2D() : Vector2D(0, 0) {}

JamJar::Vector2D::Vector2D(float x, float y) : m_x(x), m_y(y) {}

JamJar::Vector2D JamJar::Vector2D::operator+(const Vector2D &other) const {
    return Vector2D(this->m_x + other.m_x, this->m_y + other.m_y);
}

void JamJar::Vector2D::operator+=(const Vector2D &other) {
    this->m_x += other.m_x;
    this->m_y += other.m_y;
}

JamJar::Vector2D JamJar::Vector2D::operator-(const Vector2D &other) const {
    return Vector2D(this->m_x - other.m_x, this->m_y - other.m_y);
}

void JamJar::Vector2D::operator-=(const Vector2D &other) {
    this->m_x -= other.m_x;
    this->m_y -= other.m_y;
}

JamJar::Vector2D JamJar::Vector2D::operator*(const Vector2D &other) const {
    return Vector2D(this->m_x * other.m_x, this->m_y * other.m_y);
}

void JamJar::Vector2D::operator*=(const Vector2D &other) {
    this->m_x *= other.m_x;
    this->m_y *= other.m_y;
}

JamJar::Vector2D JamJar::Vector2D::operator*(float scaler) const {
    return Vector2D(this->m_x * scaler, this->m_y * scaler);
}
