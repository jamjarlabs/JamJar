#ifndef STANDARD_2D_MOTION_MOTION_SYSTEM_HPP
#define STANDARD_2D_MOTION_MOTION_SYSTEM_HPP

#include "message/message_bus.hpp"
#include "system/evaluator.hpp"
#include "system/map_system.hpp"

namespace JamJar::Standard::_2D {
class MotionSystem : public MapSystem {
  public:
    explicit MotionSystem(MessageBus *messageBus);

  protected:
    void update(float deltaTime) override;

  private:
    static bool evaluator(Entity *entity, const std::vector<JamJar::Component *> &components);
};
}; // namespace JamJar::Standard::_2D

#endif
