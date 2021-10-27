#ifndef GEOMETRY_POLYGON_HPP
#define GEOMETRY_POLYGON_HPP

#include "geometry/shape_2d.hpp"
#include "geometry/vector_2d.hpp"
#include "standard/2d/transform/transform.hpp"
#include <vector>

namespace JamJar {
class Polygon : Shape2D {
  public:
    std::vector<Vector2D> points;

    explicit Polygon(const std::vector<Vector2D> &points);
    explicit Polygon(const std::vector<float> &points);

    Vector2D Center() override;
    Vector2D FarthestPointInDir(Vector2D direction) override;
    Polygon &Transform(JamJar::Standard::_2D::Transform transform) override;
    bool PointInside(Vector2D point) override;
    std::vector<float> PointsAsFloats();
};
}; // namespace JamJar

#endif
