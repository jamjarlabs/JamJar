#ifndef STANDARD_2D_PRIMITIVE_PRIMITIVE_HPP
#define STANDARD_2D_PRIMITIVE_PRIMITIVE_HPP

#include "component/component.hpp"
#include "geometry/aabb.hpp"
#include "geometry/polygon.hpp"
#include "render/material.hpp"
#include "standard/2d/render/renderable.hpp"

namespace JamJar::Standard::_2D {
class Primitive : public Component {
  public:
    constexpr static uint32_t KEY = JamJar::hash("jamjar_2d_primitive");
    explicit Primitive(Polygon polygon, Material material, DrawMode drawMode);
    explicit Primitive(Polygon polygon, Material material);
    Polygon polygon;
    Material material;
    DrawMode drawMode;
};
}; // namespace JamJar::Standard::_2D

#endif
