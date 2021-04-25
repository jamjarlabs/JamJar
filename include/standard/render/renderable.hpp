#ifndef STANDARD_RENDER_RENDERABLE_HPP
#define STANDARD_RENDER_RENDERABLE_HPP

#include "geometry/matrix_4d.hpp"
#include "geometry/vector_2d.hpp"
#include <vector>

namespace JamJar {
namespace Standard {
struct Renderable {
    std::vector<float> m_vertices{};
    Matrix4D m_modelMatrix;
};
}; // namespace Standard
}; // namespace JamJar

#endif
