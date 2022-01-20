#ifndef STANDARD_2D_CAMERA_CAMERA_HPP
#define STANDARD_2D_CAMERA_CAMERA_HPP

#include "component/component.hpp"
#include "geometry/matrix_4d.hpp"
#include "geometry/vector_2d.hpp"
#include "render/color.hpp"
#include <SDL2/SDL.h>

namespace JamJar::Standard::_2D {
class Camera : public Component {
  public:
    static bool CanvasPositionInCameraBounds(int x, int y, Camera *camera);
    static JamJar::Vector2D CanvasPositionToViewportPosition(int x, int y, Camera *camera);
    static JamJar::Vector2D CanvasPositionToWorldPosition(int x, int y, Camera *camera);
    constexpr static uint32_t KEY = JamJar::hash("jamjar_2d_camera");
    explicit Camera();
    explicit Camera(Color backgroundColor);
    explicit Camera(Color backgroundColor, const Vector2D &viewportPosition);
    explicit Camera(Color backgroundColor, const Vector2D &viewportPosition, const Vector2D &viewportScale);
    explicit Camera(Color backgroundColor, const Vector2D &viewportPosition, const Vector2D &viewportScale,
                    const Vector2D &virtualScale);

    Color backgroundColor;
    Vector2D viewportPosition;
    Vector2D viewportScale;
    Vector2D virtualScale;

    [[nodiscard]] Matrix4D ProjectionMatrix() const;
};

static JamJar::Vector2D MousePositionToCanvasPosition(const Vector2D &mousePosition, SDL_Window *window) {
    int canvasWidth;
    int canvasHeight;

    SDL_GetWindowSize(window, &canvasWidth, &canvasHeight);

    return Vector2D((mousePosition.x / static_cast<float>(canvasWidth) - 0.5f) * 2.0f,
                    (mousePosition.y / static_cast<float>(canvasHeight) - 0.5f) * -2.0f);
}

static JamJar::Vector2D CanvasPositionToWorldPosition(const Vector2D &canvasPosition, const Vector2D &cameraPosition,
                                                      Camera *camera) {
    Vector2D cameraViewPosition = Vector2D((canvasPosition.x - camera->viewportPosition.x) / camera->viewportScale.x,
                                           (canvasPosition.y - camera->viewportPosition.y) / camera->viewportScale.y);

    return Vector2D(cameraPosition.x + camera->virtualScale.x * (cameraViewPosition.x / 2),
                    cameraPosition.y + camera->virtualScale.y * (cameraViewPosition.y / 2));
}

static JamJar::Vector2D MousePositionToWorldPosition(const Vector2D &mousePosition, const Vector2D &cameraPosition,
                                                     Camera *camera, SDL_Window *window) {
    return CanvasPositionToWorldPosition(MousePositionToCanvasPosition(mousePosition, window), cameraPosition, camera);
}

}; // namespace JamJar::Standard::_2D

#endif
