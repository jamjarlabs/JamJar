#ifndef STANDARD_2D_RENDER_RENDERABLE_HPP
#define STANDARD_2D_RENDER_RENDERABLE_HPP

#include "geometry/matrix_4d.hpp"
#include "geometry/vector_2d.hpp"
#include <vector>

namespace JamJar::Standard::_2D {
struct Renderable {
    std::vector<float> m_vertices{};
    Matrix4D m_modelMatrix;
};
}; // namespace JamJar::Standard::_2D

#endif
