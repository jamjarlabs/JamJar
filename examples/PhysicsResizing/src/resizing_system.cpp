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
    : JamJar::MapSystem(messageBus, ResizingSystem::evaluator) {}

void ResizingSystem::update(float deltaTime) {
    MapSystem::update(deltaTime);
    for (const auto &entityPair : this->entities) {
        auto entity = entityPair.second;
        auto *bodyComp =
            static_cast<JamJar::Standard::_2D::Box2DBody *>(entity.Get(JamJar::Standard::_2D::Box2DBody::KEY));
        auto *resizable = static_cast<Resizable *>(entity.Get(Resizable::KEY));

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
