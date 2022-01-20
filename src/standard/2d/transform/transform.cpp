#include "standard/2d/transform/transform.hpp"
#include "component/component.hpp"
#include "geometry/vector_2d.hpp"
#include "hash.hpp"

JamJar::Standard::_2D::Transform::Transform(const Vector2D &position, const Vector2D &scale, float angle)
    : Component(JamJar::Standard::_2D::Transform::KEY), position(position), scale(scale), angle(angle),
      previous(position) {}

JamJar::Standard::_2D::Transform::Transform(const Vector2D &position, const Vector2D &scale)
    : Transform(position, scale, 0) {}

JamJar::Standard::_2D::Transform::Transform(const Vector2D &position) : Transform(position, Vector2D(1, 1)) {}

JamJar::Standard::_2D::Transform::Transform() : Transform(Vector2D()) {}

JamJar::Matrix4D JamJar::Standard::_2D::Transform::InterpolatedMatrix4D(float alpha) const {
    auto interpolatedPos = Vector2D(this->previous.x * alpha + this->position.x * (1 - alpha),
                                    this->previous.y * alpha + this->position.y * (1 - alpha));
    auto mat = Matrix4D();
    mat.Translate(interpolatedPos);
    mat.RotateZ(this->angle);
    mat.Scale(this->scale);
    return mat;
}
