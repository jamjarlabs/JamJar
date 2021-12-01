#ifndef DESTROY_SYSTEM_HPP
#define DESTROY_SYSTEM_HPP

#include "component/component.hpp"
#include "entity/entity.hpp"
#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "system/map_system.hpp"
#include <vector>

class DestroySystem : public JamJar::MapSystem {
  public:
    explicit DestroySystem(JamJar::MessageBus *messageBus);
    void OnMessage(JamJar::Message *message) override;

  protected:
    void update(float deltaTime) override;

  private:
    static bool evaluator(JamJar::Entity *entity, const std::vector<JamJar::Component *> &components);
    float lastCreateTime;
};

#endif
