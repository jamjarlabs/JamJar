#ifndef SYSTEM_MAP_SYSTEM_HPP
#define SYSTEM_MAP_SYSTEM_HPP

#include "component/component.hpp"
#include "entity/entity.hpp"
#include "system/stateful_system.hpp"
#include "system/system_entity.hpp"
#include <map>
#include <functional>

namespace JamJar {
class MapSystem : public StatefulSystem {
  public:
    using Evaluator = std::function<bool(Entity *, const std::vector<JamJar::Component *> &)>;
    explicit MapSystem(MessageBus *messageBus, Evaluator evaluator);

  protected:
    std::map<unsigned int, SystemEntity> entities{};
    bool registerEntity(Entity *entity, std::vector<Component *> components) override;
    void removeEntity(unsigned int entityID) override;

  private:
    Evaluator evaluator{};
};
}; // namespace JamJar

#endif
