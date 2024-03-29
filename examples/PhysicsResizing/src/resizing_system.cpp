#include "resizing_system.hpp"
#include "entity/entity.hpp"
#include "resizable.hpp"
#include "standard/2d/box2d/box2d_body.hpp"
#include "standard/2d/transform/transform.hpp"

bool ResizingSystem::evaluator(JamJar::Entity *entity, const std::vector<JamJar::Component *> &components) {
    for (const auto &component : components) {
        if (component->key == Resizable::KEY) {
            return true;
        }
    }
    return false;
}

ResizingSystem::ResizingSystem(JamJar::MessageBus *messageBus)
    : JamJar::VectorSystem(messageBus, ResizingSystem::evaluator) {}

void ResizingSystem::update(float deltaTime) {
    VectorSystem::update(deltaTime);
    for (auto &entity : this->entities) {
        auto bodyComp = entity.Get<JamJar::Standard::_2D::Box2DBody>();
        auto resizable = entity.Get<Resizable>();

        auto scale = bodyComp->scale;

        if (resizable->increasing) {
            if (scale.x < resizable->max) {
                scale.x *= 1.01;
                scale.y *= 1.01;
            } else {
                scale.x *= 0.99;
                scale.y *= 0.99;
                resizable->increasing = false;
            }
        } else {
            if (scale.x > resizable->min) {
                scale.x *= 0.99;
                scale.y *= 0.99;
            } else {
                scale.x *= 1.01;
                scale.y *= 1.01;
                resizable->increasing = true;
            }
        }

        bodyComp->SetScale(scale);
    }
}
