#ifndef STANDARD_PRIMITIVE_PRIMITIVE_SYSTEM_HPP
#define STANDARD_PRIMITIVE_PRIMITIVE_SYSTEM_HPP

#include "component/component.hpp"
#include "entity/entity.hpp"
#include "message/message_bus.hpp"
#include "system/evaluator.hpp"
#include "system/map_system.hpp"

namespace JamJar {
namespace Standard {
class PrimitiveSystem : public MapSystem {
  public:
    explicit PrimitiveSystem(MessageBus *messageBus);
    void OnMessage(Message *message) override;

  private:
    void preRender(float alpha);
    static bool evaluator(Entity *entity, const std::vector<JamJar::Component *> &components);
};
}; // namespace Standard
}; // namespace JamJar

#endif
