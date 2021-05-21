#include "standard/2d/motion/motion_system.hpp"
#include "standard/2d/motion/motion.hpp"
#include "standard/2d/transform/transform.hpp"

JamJar::Standard::_2D::MotionSystem::MotionSystem(MessageBus *messageBus)
    : MapSystem(messageBus, JamJar::Standard::_2D::MotionSystem::evaluator) {}

// NOLINTNEXTLINE(misc-unused-parameters)
bool JamJar::Standard::_2D::MotionSystem::evaluator(Entity *entity,
                                                    const std::vector<JamJar::Component *> &components) {
    bool hasMotion = false;
    bool hasTransform = false;
    for (const auto &component : components) {
        if (component->m_key == JamJar::Standard::_2D::Motion::MOTION_KEY) {
            hasMotion = true;
        } else if (component->m_key == JamJar::Standard::_2D::Transform::TRANSFORM_KEY) {
            hasTransform = true;
        }

        if (hasMotion && hasTransform) {
            return true;
        }
    }
    return false;
}

void JamJar::Standard::_2D::MotionSystem::update(float deltaTime) {
    for (const auto &entityPair : this->m_entities) {
        auto entity = entityPair.second;
        auto *transform = static_cast<JamJar::Standard::_2D::Transform *>(
            entity.Get(JamJar::Standard::_2D::Transform::TRANSFORM_KEY));
        auto *motion =
            static_cast<JamJar::Standard::_2D::Motion *>(entity.Get(JamJar::Standard::_2D::Motion::MOTION_KEY));

        // v += a * dt
        motion->m_velocity += motion->m_acceleration * deltaTime;

        // p += v * dt
        transform->m_position += motion->m_velocity * deltaTime;

        // v += a * dt
        motion->m_angularVelocity += motion->m_angularAcceleration * deltaTime;
        // r += v * dt
        transform->m_angle += motion->m_angularVelocity * deltaTime;
    }
}
