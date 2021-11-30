#include "standard/2d/box2d/box2d_physics_system.hpp"
#include "message/message_payload.hpp"
#include "standard/2d/box2d/box2d_body.hpp"
#include "standard/2d/transform/transform.hpp"
#include <unordered_map>

JamJar::Standard::_2D::Box2DPhysicsSystem::Box2DContactListener::Box2DContactListener(Box2DPhysicsSystem *system)
    : system(system) {}

void JamJar::Standard::_2D::Box2DPhysicsSystem::Box2DContactListener::BeginContact(b2Contact *contact) {
    this->system->beginContact(contact);
}

void JamJar::Standard::_2D::Box2DPhysicsSystem::Box2DContactListener::EndContact(b2Contact *contact) {
    this->system->endContact(contact);
}

JamJar::Standard::_2D::Box2DPhysicsSystem::Box2DPhysicsSystem(MessageBus *messageBus, JamJar::Vector2D gravity)
    : MapSystem(messageBus, JamJar::Standard::_2D::Box2DPhysicsSystem::evaluator),
      world(b2World(b2Vec2(gravity.x, gravity.y))) {
    this->world.SetContactListener(new Box2DContactListener(this));
}

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
    MapSystem::update(deltaTime);
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

        if (bodyComp->regenerate) {
            auto fixture = body->GetFixtureList();
            b2FixtureDef fixtureDef;
            fixtureDef.density = fixture->GetDensity();
            fixtureDef.friction = fixture->GetFriction();
            fixtureDef.restitution = fixture->GetRestitution();
            fixtureDef.restitutionThreshold = fixture->GetRestitutionThreshold();
            fixtureDef.isSensor = fixture->IsSensor();
            this->createFixture(body, transform, bodyComp, &fixtureDef);
            body->DestroyFixture(fixture);
            bodyComp->regenerate = false;
        }

        auto position = body->GetPosition();
        transform->position.x = position.x;
        transform->position.y = position.y;

        transform->scale.x = bodyComp->scale.x;
        transform->scale.y = bodyComp->scale.y;

        transform->angle = body->GetAngle();
    }

    for (const auto &collision : this->collisions) {
        switch (collision.type) {
        case Box2DCollisionType::ENTER: {
            this->messageBus->Publish(
                std::make_unique<JamJar::MessagePayload<Box2DCollision>>(MESSAGE_COLLISION_ENTER, collision));
            break;
        }
        case Box2DCollisionType::EXIT: {
            this->messageBus->Publish(
                std::make_unique<JamJar::MessagePayload<Box2DCollision>>(MESSAGE_COLLISION_EXIT, collision));
            break;
        }
        }
    }
    this->collisions.clear();
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

    auto body = this->createBody(entity, transform, bodyComp);

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

b2Fixture *JamJar::Standard::_2D::Box2DPhysicsSystem::createFixture(b2Body *body, Transform *transform,
                                                                    Box2DBody *bodyComp, b2FixtureDef *def) {
    b2PolygonShape shape;

    std::vector<b2Vec2> shapePoints = std::vector<b2Vec2>();
    for (const auto &point : bodyComp->polygon.points) {
        shapePoints.push_back(b2Vec2(point.x * transform->scale.x, point.y * transform->scale.y));
    }
    shape.Set(shapePoints.data(), shapePoints.size());
    def->shape = &shape;
    return body->CreateFixture(def);
}

b2Fixture *JamJar::Standard::_2D::Box2DPhysicsSystem::createFixture(b2Body *body, Transform *transform,
                                                                    Box2DBody *bodyComp) {

    b2FixtureDef fixtureDef;
    fixtureDef.density = bodyComp->initializationProperties.density;
    fixtureDef.friction = bodyComp->initializationProperties.friction;
    fixtureDef.restitution = bodyComp->initializationProperties.restitution;
    fixtureDef.restitutionThreshold = bodyComp->initializationProperties.restitutionThreshold;
    fixtureDef.isSensor = bodyComp->initializationProperties.isSensor;

    return this->createFixture(body, transform, bodyComp, &fixtureDef);
}

b2Body *JamJar::Standard::_2D::Box2DPhysicsSystem::createBody(Entity *entity, Transform *transform,
                                                              Box2DBody *bodyComp) {

    bodyComp->scale = transform->scale;

    b2BodyDef bodyDef;
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(entity);
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

    this->createFixture(body, transform, bodyComp);

    return body;
}

JamJar::Entity *getEntityFromFixture(b2Fixture *fixture) {
    b2Body *body = fixture->GetBody();
    b2BodyUserData userData = body->GetUserData();
    return (JamJar::Entity *)userData.pointer;
}

void JamJar::Standard::_2D::Box2DPhysicsSystem::Box2DPhysicsSystem::beginContact(b2Contact *contact) {
    auto aEntity = getEntityFromFixture(contact->GetFixtureA());
    auto bEntity = getEntityFromFixture(contact->GetFixtureB());
    this->collisions.push_back(Box2DCollision(
        {.aID = aEntity->id, .bID = bEntity->id, .contact = contact, .type = Box2DCollisionType::ENTER}));
}

void JamJar::Standard::_2D::Box2DPhysicsSystem::Box2DPhysicsSystem::endContact(b2Contact *contact) {
    auto aEntity = getEntityFromFixture(contact->GetFixtureA());
    auto bEntity = getEntityFromFixture(contact->GetFixtureB());
    this->collisions.push_back(
        Box2DCollision({.aID = aEntity->id, .bID = bEntity->id, .contact = contact, .type = Box2DCollisionType::EXIT}));
}
