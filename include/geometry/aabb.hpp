#ifndef GEOMETRY_AABB_HPP
#define GEOMETRY_AABB_HPP

#include "geometry/shape_2d.hpp"
#include "geometry/vector_2d.hpp"
#include "standard/2d/transform/transform.hpp"

namespace JamJar {
class AABB : Shape2D {
  public:
    Vector2D dimensions;
    Vector2D center;

    explicit AABB(const Vector2D &dimensions);
    explicit AABB(const Vector2D &dimensions, const Vector2D &center);

    Vector2D Center() override;
    Vector2D FarthestPointInDir(Vector2D direction) override;
    AABB &Transform(JamJar::Standard::_2D::Transform transform) override;
    bool PointInside(Vector2D point) override;
};
}; // namespace JamJar

#endif
