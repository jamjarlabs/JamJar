#ifndef STANDARD_2D_PRIMITIVE_PRIMITIVE_HPP
#define STANDARD_2D_PRIMITIVE_PRIMITIVE_HPP

#include "component/component.hpp"
#include "geometry/vector_2d.hpp"
#include <vector>

namespace JamJar::Standard::_2D {
class Primitive : public Component {
  public:
    constexpr static uint32_t PRIMITIVE_KEY = JamJar::hash("jamjar_primitive");
    explicit Primitive(std::vector<float> points);
    std::vector<float> m_points{};
};
}; // namespace JamJar::Standard::_2D

#endif
