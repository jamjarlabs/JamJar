#ifndef STANDARD_2D_BOX2D_BOX2D_PHYSICS_SYSTEM_HPP
#define STANDARD_2D_BOX2D_BOX2D_PHYSICS_SYSTEM_HPP

#include "box2d/box2d.h"
#include "geometry/vector_2d.hpp"
#include "hash.hpp"
#include "message/message_bus.hpp"
#include "standard/2d/box2d/box2d_body.hpp"
#include "standard/2d/transform/transform.hpp"
#include "system/vector_system.hpp"
#include <map>

namespace JamJar::Standard::_2D {

enum Box2DCollisionType { ENTER, EXIT };
struct Box2DCollision {
    unsigned int aID;
    unsigned int bID;
    b2Contact *contact;
    Box2DCollisionType type;
};
class Box2DPhysicsSystem : public VectorSystem {
  public:
    constexpr static uint32_t MESSAGE_COLLISION_ENTER = hash("jamjar_box2d_collision_enter");
    constexpr static uint32_t MESSAGE_COLLISION_EXIT = hash("jamjar_box2d_collision_exit");
    explicit Box2DPhysicsSystem(MessageBus *messageBus, const JamJar::Vector2D &gravity);
    void OnMessage(Message *message) override;

  protected:
    void update(float deltaTime) override;
    bool registerEntity(Entity *entity, std::vector<Component *> components) override;
    void removeEntity(unsigned int entityID) override;

  private:
    void beginContact(b2Contact *contact);
    void endContact(b2Contact *contact);
    b2Fixture *createFixture(b2Body *body, Transform *transform, Box2DBody *bodyComp, b2FixtureDef *def);
    b2Fixture *createFixture(b2Body *body, Transform *transform, Box2DBody *bodyComp);
    b2Body *createBody(Entity *entity, Transform *transform, Box2DBody *bodyComp);
    b2World world;
    std::map<unsigned int, b2Body *> bodies;
    std::vector<Box2DCollision> collisions;
    static bool evaluator(Entity *entity, const std::vector<JamJar::Component *> &components);
    class Box2DContactListener : public b2ContactListener {
      public:
        explicit Box2DContactListener(Box2DPhysicsSystem *system);
        void BeginContact(b2Contact *contact) override;
        void EndContact(b2Contact *contact) override;

      private:
        Box2DPhysicsSystem *system;
    };
};
}; // namespace JamJar::Standard::_2D

#endif
