#ifndef STANDARD_2D_BOX2D_BOX2D_PHYSICS_SYSTEM_HPP
#define STANDARD_2D_BOX2D_BOX2D_PHYSICS_SYSTEM_HPP

#include "box2d/box2d.h"
#include "geometry/vector_2d.hpp"
#include "message/message_bus.hpp"
#include "standard/2d/box2d/box2d_body.hpp"
#include "standard/2d/transform/transform.hpp"
#include "system/evaluator.hpp"
#include "system/map_system.hpp"
#include <map>

namespace JamJar::Standard::_2D {
class Box2DPhysicsSystem : public MapSystem {
  public:
    explicit Box2DPhysicsSystem(MessageBus *messageBus, JamJar::Vector2D gravity);
    void OnMessage(Message *message) override;

  protected:
    void update(float deltaTime) override;
    bool registerEntity(Entity *entity, std::vector<Component *> components) override;
    void removeEntity(unsigned int entityID) override;

  private:
    b2Fixture *createFixture(b2Body *body, Transform *transform, Box2DBody *bodyComp, b2FixtureDef *def);
    b2Fixture *createFixture(b2Body *body, Transform *transform, Box2DBody *bodyComp);
    b2Body *createBody(Transform *transform, Box2DBody *bodyComp);
    b2World world;
    std::map<unsigned int, b2Body *> bodies;
    static bool evaluator(Entity *entity, const std::vector<JamJar::Component *> &components);
};
}; // namespace JamJar::Standard::_2D

#endif
