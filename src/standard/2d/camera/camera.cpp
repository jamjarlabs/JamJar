#include "standard/2d/camera/camera.hpp"
#include "geometry/matrix_4d.hpp"
#include "geometry/vector_2d.hpp"
#include "render/color.hpp"

const float DEFAULT_VIRTUAL_WIDTH = 160;
const float DEFAULT_VIRTUAL_HEIGHT = 90;

const float DEFAULT_BG_RED = 0;
const float DEFAULT_BG_GREEN = 0;
const float DEFAULT_BG_BLUE = 0;
const float DEFAULT_BG_ALPHA = 1;

const float DEFAULT_VIEWPORT_POSITION_X = 0;
const float DEFAULT_VIEWPORT_POSITION_Y = 0;

const float DEFAULT_VIEWPORT_SCALE_X = 1;
const float DEFAULT_VIEWPORT_SCALE_Y = 1;

const float ORTHOGRAPHIC_NEAR = 0;
const float ORTHOGRAPHIC_FAR = 100;

JamJar::Standard::_2D::Camera::Camera(Color backgroundColor, Vector2D viewportPosition, Vector2D viewportScale,
                                      Vector2D virtualScale)
    : Component(JamJar::Standard::_2D::Camera::CAMERA_KEY), m_backgroundColor(backgroundColor),
      m_viewportPosition(viewportPosition), m_viewportScale(viewportScale), m_virtualScale(virtualScale) {}

JamJar::Standard::_2D::Camera::Camera(Color backgroundColor, Vector2D viewportPosition, Vector2D viewportScale)
    : Camera(backgroundColor, viewportPosition, viewportScale,
             Vector2D(DEFAULT_VIRTUAL_WIDTH, DEFAULT_VIRTUAL_HEIGHT)) {}

JamJar::Standard::_2D::Camera::Camera(Color backgroundColor, Vector2D viewportPosition)
    : Camera(backgroundColor, viewportPosition, Vector2D(DEFAULT_VIEWPORT_SCALE_X, DEFAULT_VIEWPORT_SCALE_Y)) {}

JamJar::Standard::_2D::Camera::Camera(Color backgroundColor)
    : Camera(backgroundColor, Vector2D(DEFAULT_VIEWPORT_POSITION_X, DEFAULT_VIEWPORT_POSITION_Y)) {}

JamJar::Standard::_2D::Camera::Camera()
    : Camera(Color(DEFAULT_BG_RED, DEFAULT_BG_GREEN, DEFAULT_BG_BLUE, DEFAULT_BG_ALPHA)) {}

JamJar::Matrix4D JamJar::Standard::_2D::Camera::ProjectionMatrix() const {
    auto halfWidth = this->m_virtualScale.m_x / 2;
    auto halfHeight = this->m_virtualScale.m_y / 2;
    return JamJar::Matrix4D::Ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, ORTHOGRAPHIC_NEAR, ORTHOGRAPHIC_FAR);
}
