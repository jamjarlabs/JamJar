#include "standard/2d/motion/motion_system.hpp"
#include "standard/2d/motion/motion.hpp"
#include "standard/2d/transform/transform.hpp"

JamJar::Standard::_2D::MotionSystem::MotionSystem(MessageBus *messageBus)
    : MapSystem(messageBus, JamJar::Standard::_2D::MotionSystem::evaluator) {}

bool JamJar::Standard::_2D::MotionSystem::evaluator(Entity *entity,
                                                    const std::vector<JamJar::Component *> &components) {
    bool hasMotion = false;
    bool hasTransform = false;
    for (const auto &component : components) {
        if (component->key == JamJar::Standard::_2D::Motion::KEY) {
            hasMotion = true;
        } else if (component->key == JamJar::Standard::_2D::Transform::KEY) {
            hasTransform = true;
        }

        if (hasMotion && hasTransform) {
            return true;
        }
    }
    return false;
}

void JamJar::Standard::_2D::MotionSystem::update(float deltaTime) {
    for (const auto &entityPair : this->entities) {
        auto entity = entityPair.second;
        auto *transform =
            static_cast<JamJar::Standard::_2D::Transform *>(entity.Get(JamJar::Standard::_2D::Transform::KEY));
        auto *motion = static_cast<JamJar::Standard::_2D::Motion *>(entity.Get(JamJar::Standard::_2D::Motion::KEY));

        // v += a * dt
        motion->velocity += motion->acceleration * deltaTime;

        // p += v * dt
        transform->position += motion->velocity * deltaTime;

        // v += a * dt
        motion->angularVelocity += motion->angularAcceleration * deltaTime;
        // r += v * dt
        transform->angle += motion->angularVelocity * deltaTime;
    }
}
