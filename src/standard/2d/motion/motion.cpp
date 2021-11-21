#include "standard/2d/motion/motion.hpp"

JamJar::Standard::_2D::Motion::Motion(Vector2D velocity, Vector2D acceleration, float angularVelocity,
                                      float angularAcceleration)
    : Component(JamJar::Standard::_2D::Motion::KEY), velocity(velocity), acceleration(acceleration),
      angularVelocity(angularVelocity), angularAcceleration(angularAcceleration) {}

JamJar::Standard::_2D::Motion::Motion(Vector2D velocity, Vector2D acceleration, float angularVelocity)
    : Motion(velocity, acceleration, angularVelocity, 0) {}

JamJar::Standard::_2D::Motion::Motion(Vector2D velocity, Vector2D acceleration)
    : Motion(velocity, acceleration, 0, 0) {}

JamJar::Standard::_2D::Motion::Motion(Vector2D velocity) : Motion(velocity, Vector2D(), 0, 0) {}

JamJar::Standard::_2D::Motion::Motion() : Motion(Vector2D(), Vector2D(), 0, 0) {}
