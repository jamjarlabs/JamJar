#ifndef GEOMETRY_MATRIX_4D_HPP
#define GEOMETRY_MATRIX_4D_HPP

#include "geometry/vector_2d.hpp"
#include <vector>

namespace JamJar {
class Matrix4D {
  public:
    std::vector<float> data{};
    Matrix4D();
    explicit Matrix4D(const std::vector<float> &data);
    Matrix4D *Translate(const Vector2D &translation);
    Matrix4D *Scale(const Vector2D &scale);
    Matrix4D *RotateZ(const float &rotation);
    Matrix4D *RotateZDeg(const float &rotation);
    static Matrix4D Ortho(float left, float right, float bottom, float top, float near, float far);
};
}; // namespace JamJar

#endif
