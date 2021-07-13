#ifndef STANDARD_2D_RENDER_RENDERABLE_HPP
#define STANDARD_2D_RENDER_RENDERABLE_HPP

#include "geometry/matrix_4d.hpp"
#include "geometry/vector_2d.hpp"
#include "render/material.hpp"
#include <string>
#include <vector>

namespace JamJar::Standard::_2D {
enum DrawMode { POINTS, LINES, LINE_STRIP, TRIANGLES, TRIANGLE_STRIP };

struct Renderable {
    std::vector<float> vertices{};
    Matrix4D modelMatrix;
    Material material;
    DrawMode drawMode;
};
}; // namespace JamJar::Standard::_2D

#endif
