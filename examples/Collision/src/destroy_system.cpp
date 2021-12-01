#include "destroy_system.hpp"
#include "box.hpp"
#include "destructor.hpp"
#include "entity/entity.hpp"
#include "geometry/polygon.hpp"
#include "geometry/vector_2d.hpp"
#include "message/message_payload.hpp"
#include "render/color.hpp"
#include "render/texture.hpp"
#include "standard/2d/box2d/box2d_body.hpp"
#include "standard/2d/box2d/box2d_physics_system.hpp"
#include "standard/2d/primitive/primitive.hpp"
#include "standard/2d/transform/transform.hpp"

const float TIME_BETWEEN_BOXES = 3;

bool DestroySystem::evaluator(JamJar::Entity *entity, const std::vector<JamJar::Component *> &components) {
    bool hasBox2dBody = false;
    bool hasBoxOrDestructor = false;
    for (const auto &component : components) {
        if (component->key == JamJar::Standard::_2D::Box2DBody::KEY) {
            hasBox2dBody = true;
            continue;
        }
        if (component->key == Box::KEY) {
            hasBoxOrDestructor = true;
            continue;
        }
        if (component->key == Destructor::KEY) {
            hasBoxOrDestructor = true;
            continue;
        }
    }
    return hasBox2dBody && hasBoxOrDestructor;
}

DestroySystem::DestroySystem(JamJar::MessageBus *messageBus)
    : JamJar::MapSystem(messageBus, DestroySystem::evaluator), lastCreateTime(0) {
    this->messageBus->Subscribe(this, JamJar::Standard::_2D::Box2DPhysicsSystem::MESSAGE_COLLISION_ENTER);
}

void DestroySystem::OnMessage(JamJar::Message *message) {
    MapSystem::OnMessage(message);
    switch (message->type) {
    case JamJar::Standard::_2D::Box2DPhysicsSystem::MESSAGE_COLLISION_ENTER: {
        auto *collisionMessage = static_cast<JamJar::MessagePayload<JamJar::Standard::_2D::Box2DCollision> *>(message);
        auto collision = collisionMessage->payload;
        if (this->entities.count(collision.aID) == 0 || this->entities.count(collision.bID) == 0) {
            break;
        }

        auto a = this->entities.at(collision.aID);
        auto b = this->entities.at(collision.bID);

        if (a.Has(Box::KEY)) {
            if (b.Has(Destructor::KEY)) {
                a.Destroy();
                break;
            }
        }

        if (b.Has(Box::KEY)) {
            if (a.Has(Destructor::KEY)) {
                b.Destroy();
                break;
            }
        }

        break;
    }
    }
}

void DestroySystem::update(float deltaTime) {
    MapSystem::update(deltaTime);
    this->lastCreateTime += deltaTime;

    if (this->lastCreateTime > TIME_BETWEEN_BOXES) {
        auto box = new JamJar::Entity(messageBus);
        box->Add(std::move(
            std::make_unique<JamJar::Standard::_2D::Transform>(JamJar::Vector2D(-50, 50), JamJar::Vector2D(10, 10))));
        box->Add(std::move(std::make_unique<JamJar::Standard::_2D::Box2DBody>(
            JamJar::Polygon({-0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, -0.5}),
            JamJar::Standard::_2D::Box2DBodyProperties({.density = 1.0f, .angularVelocity = -1.0f}))));
        box->Add(std::move(std::make_unique<JamJar::Standard::_2D::Primitive>(
            JamJar::Polygon({-0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5}),
            JamJar::Material(JamJar::Color(0, 1, 0, 1)))));
        box->Add(std::move(std::make_unique<Box>()));
        this->lastCreateTime = 0;
    }
}
