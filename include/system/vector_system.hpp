#ifndef SYSTEM_VECTOR_SYSTEM_HPP
#define SYSTEM_VECTOR_SYSTEM_HPP

#include "component/component.hpp"
#include "entity/entity.hpp"
#include "system/stateful_system.hpp"
#include "system/system_entity.hpp"
#include <vector>
#include <functional>

namespace JamJar {
class VectorSystem : public StatefulSystem {
  public:
    using Evaluator = std::function<bool(Entity *, const std::vector<JamJar::Component *> &)>;
    explicit VectorSystem(MessageBus *messageBus, Evaluator evaluator);

  protected:
    std::vector<SystemEntity> entities{};
    bool registerEntity(Entity *entity, std::vector<Component *> components) override;
    void removeEntity(unsigned int entityID) override;

  private:
    bool isEntityRegistered(unsigned int entityID);
    Evaluator evaluator{};
};
}; // namespace JamJar

#endif
