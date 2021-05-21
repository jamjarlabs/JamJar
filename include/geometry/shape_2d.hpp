#ifndef GEOMETRY_SHAPE_2D_HPP
#define GEOMETRY_SHAPE_2D_HPP

#include "geometry/vector_2d.hpp"
#include "standard/2d/transform/transform.hpp"

namespace JamJar {
class Shape2D {
  public:
    virtual Vector2D Center() = 0;
    virtual Vector2D FarthestPointInDir(Vector2D direction) = 0;
    virtual Shape2D &Transform(JamJar::Standard::_2D::Transform transform) = 0;
    virtual bool PointInside(Vector2D point) = 0;
};
}; // namespace JamJar

#endif
