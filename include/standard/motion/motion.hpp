#ifndef STANDARD_MOTION_MOTION_HPP
#define STANDARD_MOTION_MOTION_HPP

#include "component/component.hpp"
#include "geometry/vector_2d.hpp"

namespace JamJar {
namespace Standard {
class Motion : public Component {
  public:
    constexpr static uint32_t MOTION_KEY = JamJar::hash("jamjar_motion");
    Motion();
    explicit Motion(Vector2D velocity);
    explicit Motion(Vector2D velocity, Vector2D acceleration);
    explicit Motion(Vector2D velocity, Vector2D acceleration, float angularVelocity);
    explicit Motion(Vector2D velocity, Vector2D acceleration, float angularVelocity, float angularAcceleration);
    Vector2D m_velocity;
    Vector2D m_acceleration;
    float m_angularVelocity;
    float m_angularAcceleration;
};
}; // namespace Standard
}; // namespace JamJar

#endif
