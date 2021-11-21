#ifndef SYSTEM_STATEFUL_SYSTEM_HPP
#define SYSTEM_STATEFUL_SYSTEM_HPP

#include "component/component.hpp"
#include "entity/entity.hpp"
#include "system/system.hpp"

namespace JamJar {
class StatefulSystem : public System {
  protected:
    virtual bool registerEntity(Entity *entity, std::vector<Component *> components) = 0;
    virtual void removeEntity(unsigned int entityID) = 0;

  public:
    explicit StatefulSystem(MessageBus *messageBus);
    void OnMessage(Message *message) override;
};
}; // namespace JamJar

#endif
