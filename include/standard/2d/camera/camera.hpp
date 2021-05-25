#ifndef STANDARD_2D_CAMERA_CAMERA_HPP
#define STANDARD_2D_CAMERA_CAMERA_HPP

#include "component/component.hpp"
#include "geometry/matrix_4d.hpp"
#include "geometry/vector_2d.hpp"
#include "render/color.hpp"

namespace JamJar::Standard::_2D {
class Camera : public Component {
  public:
    constexpr static uint32_t KEY = JamJar::hash("jamjar_2d_camera");
    explicit Camera();
    explicit Camera(Color backgroundColor);
    explicit Camera(Color backgroundColor, Vector2D viewportPosition);
    explicit Camera(Color backgroundColor, Vector2D viewportPosition, Vector2D viewportScale);
    explicit Camera(Color backgroundColor, Vector2D viewportPosition, Vector2D viewportScale, Vector2D virtualScale);

    Color backgroundColor;
    Vector2D viewportPosition;
    Vector2D viewportScale;
    Vector2D virtualScale;

    [[nodiscard]] Matrix4D ProjectionMatrix() const;
};
}; // namespace JamJar::Standard::_2D

#endif
