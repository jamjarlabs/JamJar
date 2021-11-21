#include "standard/2d/box2d/box2d_physics_system.hpp"
#include "standard/2d/box2d/box2d_body.hpp"
#include "standard/2d/transform/transform.hpp"
#include <iostream>
#include <unordered_map>

JamJar::Standard::_2D::Box2DPhysicsSystem::Box2DPhysicsSystem(MessageBus *messageBus, JamJar::Vector2D gravity)
    : MapSystem(messageBus, JamJar::Standard::_2D::Box2DPhysicsSystem::evaluator),
      world(b2World(b2Vec2(gravity.x, gravity.y))) {}

bool JamJar::Standard::_2D::Box2DPhysicsSystem::evaluator(Entity *entity,
                                                          const std::vector<JamJar::Component *> &components) {
    bool hasBody = false;
    bool hasTransform = false;
    for (const auto &component : components) {
        if (component->key == JamJar::Standard::_2D::Box2DBody::KEY) {
            hasBody = true;
        } else if (component->key == JamJar::Standard::_2D::Transform::KEY) {
            hasTransform = true;
        }

        if (hasBody && hasTransform) {
            return true;
        }
    }
    return false;
}

void JamJar::Standard::_2D::Box2DPhysicsSystem::OnMessage(JamJar::Message *message) { MapSystem::OnMessage(message); }

void JamJar::Standard::_2D::Box2DPhysicsSystem::update(float deltaTime) {
    world.Step(deltaTime, 6, 2);
    for (const auto &entityPair : this->entities) {
        auto entity = entityPair.second;
        auto *transform =
            static_cast<JamJar::Standard::_2D::Transform *>(entity.Get(JamJar::Standard::_2D::Transform::KEY));
        auto *bodyComp =
            static_cast<JamJar::Standard::_2D::Box2DBody *>(entity.Get(JamJar::Standard::_2D::Box2DBody::KEY));

        if (bodies.count(entity.entity->id) == 0) {
            continue;
        }

        b2Body *body = bodies.at(entity.entity->id);
        auto position = body->GetPosition();
        transform->position.x = position.x;
        transform->position.y = position.y;

        transform->angle = body->GetAngle();
    }
}

bool JamJar::Standard::_2D::Box2DPhysicsSystem::registerEntity(Entity *entity, std::vector<Component *> components) {
    if (!MapSystem::registerEntity(entity, components)) {
        return false;
    }

    JamJar::Standard::_2D::Box2DBody *bodyComp = nullptr;
    JamJar::Standard::_2D::Transform *transform = nullptr;
    for (const auto &component : components) {
        if (component->key == JamJar::Standard::_2D::Box2DBody::KEY) {
            bodyComp = static_cast<JamJar::Standard::_2D::Box2DBody *>(component);
        } else if (component->key == JamJar::Standard::_2D::Transform::KEY) {
            transform = static_cast<JamJar::Standard::_2D::Transform *>(component);
        }
    }

    b2BodyDef bodyDef;
    bodyDef.position.Set(transform->position.x, transform->position.y);
    bodyDef.type = Box2DBody::BODY_TYPES.at(bodyComp->initializationProperties.bodyType);
    bodyDef.linearVelocity = b2Vec2(bodyComp->initializationProperties.linearVelocity.x,
                                    bodyComp->initializationProperties.linearVelocity.y);
    bodyDef.angularVelocity = bodyComp->initializationProperties.angularVelocity;
    bodyDef.linearDamping = bodyComp->initializationProperties.linearDamping;
    bodyDef.angularDamping = bodyComp->initializationProperties.angularDamping;
    bodyDef.gravityScale = bodyComp->initializationProperties.gravityScale;
    bodyDef.allowSleep = bodyComp->initializationProperties.allowSleep;
    bodyDef.awake = bodyComp->initializationProperties.awake;
    bodyDef.fixedRotation = bodyComp->initializationProperties.fixedRotation;
    bodyDef.bullet = bodyComp->initializationProperties.bullet;
    bodyDef.enabled = bodyComp->initializationProperties.enabled;

    b2Body *body = world.CreateBody(&bodyDef);
    b2PolygonShape shape;

    std::vector<b2Vec2> shapePoints = std::vector<b2Vec2>();
    for (const auto &point : bodyComp->polygon.points) {
        shapePoints.push_back(b2Vec2(point.x * transform->scale.x, point.y  * transform->scale.y));
    }
    shape.Set(shapePoints.data(), shapePoints.size());

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = bodyComp->initializationProperties.density;
    fixtureDef.friction = bodyComp->initializationProperties.friction;
    fixtureDef.restitution = bodyComp->initializationProperties.restitution;
    fixtureDef.restitutionThreshold = bodyComp->initializationProperties.restitutionThreshold;
    fixtureDef.isSensor = bodyComp->initializationProperties.isSensor;
    body->CreateFixture(&fixtureDef);
    bodies[entity->id] = body;
    bodyComp->SetBody(body);

    return true;
}

void JamJar::Standard::_2D::Box2DPhysicsSystem::removeEntity(unsigned int entityID) {
    MapSystem::removeEntity(entityID);
    b2Body *body = bodies[entityID];
    if (body != nullptr) {
        world.DestroyBody(body);
    }
    bodies.erase(entityID);
}
