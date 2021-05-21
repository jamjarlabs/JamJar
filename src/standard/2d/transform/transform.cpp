#include "standard/2d/transform/transform.hpp"
#include "component/component.hpp"
#include "geometry/vector_2d.hpp"
#include "hash.hpp"

JamJar::Standard::_2D::Transform::Transform(Vector2D position, Vector2D scale, float angle)
    : Component(JamJar::Standard::_2D::Transform::TRANSFORM_KEY), m_position(position), m_scale(scale), m_angle(angle),
      m_previous(position) {}

JamJar::Standard::_2D::Transform::Transform(Vector2D position, Vector2D scale) : Transform(position, scale, 0) {}

JamJar::Standard::_2D::Transform::Transform(Vector2D position) : Transform(position, Vector2D(1, 1)) {}

JamJar::Standard::_2D::Transform::Transform() : Transform(Vector2D()) {}

JamJar::Matrix4D JamJar::Standard::_2D::Transform::InterpolatedMatrix4D(float alpha) const {
    auto interpolatedPos = Vector2D(this->m_previous.m_x * alpha + this->m_position.m_x * (1 - alpha),
                                    this->m_previous.m_y * alpha + this->m_position.m_y * (1 - alpha));
    auto mat = Matrix4D();
    mat.Translate(interpolatedPos);
    mat.Rotate(this->m_angle);
    mat.Scale(this->m_scale);
    return mat;
}
