#include "standard/primitive/primitive.hpp"
#include <utility>
#include <vector>

JamJar::Standard::Primitive::Primitive(std::vector<float> points)
    : Component(Primitive::PRIMITIVE_KEY), m_points(std::move(points)) {}
