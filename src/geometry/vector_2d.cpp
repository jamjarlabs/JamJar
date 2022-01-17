#include "geometry/vector_2d.hpp"
#include <cmath>

JamJar::Vector2D::Vector2D() : Vector2D(0, 0) {}

JamJar::Vector2D::Vector2D(float x, float y) : x(x), y(y) {}

JamJar::Vector2D::Vector2D(const Vector2D &vec) : Vector2D(vec.x, vec.y) {}

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

void JamJar::Vector2D::operator*=(float scaler) {
    this->x *= scaler;
    this->y *= scaler;
}

float JamJar::Vector2D::Dot(const Vector2D &other) const { return this->x * other.x + this->y * other.y; }

float JamJar::Vector2D::Magnitude() const { return sqrtf(this->x * this->x + this->y * this->y); }

float JamJar::Vector2D::Distance(const Vector2D &other) const {
    float x = this->x - other.x;
    float y = this->y - other.y;
    return std::abs(sqrtf(x * x + y * y));
}

JamJar::Vector2D JamJar::Vector2D::Rotate(const Vector2D &center, float angle) const {
    Vector2D copy = *this;
    return *copy.RotateInPlace(center, angle);
}

JamJar::Vector2D JamJar::Vector2D::RotateDeg(const Vector2D &center, float angle) const {
    Vector2D copy = *this;
    return *copy.RotateDegInPlace(center, angle);
}

JamJar::Vector2D JamJar::Vector2D::Invert() const {
    Vector2D copy = *this;
    return *copy.InvertInPlace();
}

JamJar::Vector2D JamJar::Vector2D::Normalize() const {
    Vector2D copy = *this;
    return *copy.NormalizeInPlace();
}

JamJar::Vector2D *JamJar::Vector2D::RotateInPlace(const Vector2D &center, float angle) {
    float s = sinf(angle);
    float c = cosf(angle);

    float x = this->x - center.x;
    float y = this->y - center.y;

    this->x = (x * c - y * s) + center.x;
    this->y = (x * s + y * c) + center.y;

    return this;
}

JamJar::Vector2D *JamJar::Vector2D::RotateDegInPlace(const Vector2D &center, float angle) {
    return this->RotateInPlace(center, angle * (M_PI / 180.0f));
}

JamJar::Vector2D *JamJar::Vector2D::InvertInPlace() {
    this->x = -this->x;
    this->y = -this->y;
    return this;
}

JamJar::Vector2D *JamJar::Vector2D::NormalizeInPlace() {
    float magnitude = this->Magnitude();
    if (magnitude > 0) {
        magnitude = 1.0f / magnitude;
    }

    (*this) *= magnitude;

    return this;
}
