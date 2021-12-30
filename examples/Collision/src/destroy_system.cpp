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

std::optional<uint32_t> DestroySystem::evaluator(JamJar::Entity *entity, const std::vector<JamJar::Component *> &components) {
    bool hasBox2dBody = false;
    bool hasBox = false;
    bool hasDestructor = false;
    for (const auto &component : components) {
        if (component->key == JamJar::Standard::_2D::Box2DBody::KEY) {
            hasBox2dBody = true;
            continue;
        }
        if (component->key == Box::KEY) {
            hasBox = true;
            continue;
        }
        if (component->key == Destructor::KEY) {
            hasDestructor = true;
            continue;
        }
    }

    if (hasBox2dBody) {
        if (hasBox) {
            return BOX_BUCKET;
        }

        if (hasDestructor) {
            return DESTRUCTOR_BUCKET;
        }
    }

    return std::optional<uint32_t>(std::nullopt);
}

DestroySystem::DestroySystem(JamJar::MessageBus *messageBus)
    : JamJar::BucketMapSystem(messageBus, DestroySystem::evaluator), lastCreateTime(0) {
    this->messageBus->Subscribe(this, JamJar::Standard::_2D::Box2DPhysicsSystem::MESSAGE_COLLISION_ENTER);
}

void DestroySystem::OnMessage(JamJar::Message *message) {
    BucketMapSystem::OnMessage(message);
    switch (message->type) {
    case JamJar::Standard::_2D::Box2DPhysicsSystem::MESSAGE_COLLISION_ENTER: {
        auto *collisionMessage = static_cast<JamJar::MessagePayload<JamJar::Standard::_2D::Box2DCollision> *>(message);
        auto collision = collisionMessage->payload;

        if (this->entities.count(BOX_BUCKET) == 0 || this->entities.count(DESTRUCTOR_BUCKET) == 0) {
            break;
        }

        auto boxes = this->entities.at(BOX_BUCKET);
        auto destructors = this->entities.at(DESTRUCTOR_BUCKET);

        if (boxes.count(collision.aID) > 0) {
            // A is a Box
            if (destructors.count(collision.bID) > 0) {
                // B is a destructor
                auto a = boxes.at(collision.aID);
                a.Destroy();
                break;
            }
        } else if (destructors.count(collision.aID) > 0) {
            // A is a destructor
            if (boxes.count(collision.bID) > 0) {
                // B is a box
                auto b = boxes.at(collision.bID);
                b.Destroy();
                break;
            }
        }
        break;
    }
    }
}

void DestroySystem::update(float deltaTime) {
    BucketMapSystem::update(deltaTime);
    this->lastCreateTime += deltaTime;

    if (this->lastCreateTime > TIME_BETWEEN_BOXES) {
        auto box = new JamJar::Entity(messageBus);
        box->Add(new JamJar::Standard::_2D::Transform(JamJar::Vector2D(-50, 50), JamJar::Vector2D(10, 10)));
        box->Add(new JamJar::Standard::_2D::Box2DBody(
            JamJar::Polygon({-0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, -0.5}),
            JamJar::Standard::_2D::Box2DBodyProperties({.density = 1.0f, .angularVelocity = -1.0f})));
        box->Add(new JamJar::Standard::_2D::Primitive(
            JamJar::Polygon({-0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5}),
            JamJar::Material(JamJar::Color(0, 1, 0, 1))));
        box->Add(new Box());
        this->lastCreateTime = 0;
    }
}
