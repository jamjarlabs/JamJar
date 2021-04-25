#ifndef STANDARD_INTERPOLATION_INTERPOLATION_SYSTEM_HPP
#define STANDARD_INTERPOLATION_INTERPOLATION_SYSTEM_HPP

#include "message/message_bus.hpp"
#include "system/evaluator.hpp"
#include "system/map_system.hpp"

namespace JamJar {
namespace Standard {
class InterpolationSystem : public MapSystem {
  public:
    explicit InterpolationSystem(MessageBus *messageBus);
    void OnMessage(Message *message) override;

  private:
    void interpolate();
    static bool evaluator(Entity *entity, const std::vector<JamJar::Component *> &components);
};
}; // namespace Standard
}; // namespace JamJar

#endif
