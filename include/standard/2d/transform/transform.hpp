#ifndef STANDARD_2D_TRANSFORM_TRANSFORM_HPP
#define STANDARD_2D_TRANSFORM_TRANSFORM_HPP

#include "component/component.hpp"
#include "geometry/matrix_4d.hpp"
#include "geometry/vector_2d.hpp"

namespace JamJar::Standard::_2D {
class Transform : public Component {
  public:
    constexpr static uint32_t KEY = JamJar::hash("jamjar_2d_transform");
    Transform();
    explicit Transform(const Vector2D &position);
    explicit Transform(const Vector2D &position, const Vector2D &scale);
    explicit Transform(const Vector2D &position, const Vector2D &scale, float angle);
    Vector2D previous;
    Vector2D position;
    Vector2D scale;
    float angle;
    [[nodiscard]] Matrix4D InterpolatedMatrix4D(float alpha) const;
};
}; // namespace JamJar::Standard::_2D

#endif
