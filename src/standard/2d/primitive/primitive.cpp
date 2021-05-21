#include "standard/2d/primitive/primitive.hpp"
#include <utility>
#include <vector>

JamJar::Standard::_2D::Primitive::Primitive(std::vector<float> points)
    : Component(Primitive::PRIMITIVE_KEY), m_points(std::move(points)) {}
