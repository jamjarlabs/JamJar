#ifndef SYSTEM_MAP_SYSTEM_HPP
#define SYSTEM_MAP_SYSTEM_HPP

#include "component/component.hpp"
#include "entity/entity.hpp"
#include "system/evaluator.hpp"
#include "system/stateful_system.hpp"
#include "system/system_entity.hpp"

namespace JamJar {
class MapSystem : public StatefulSystem {
  private:
    Evaluator m_evaluator{};

  protected:
    std::map<unsigned int, SystemEntity> entities{};
    void registerEntity(Entity *entity, std::vector<Component *> components) override;
    void removeEntity(unsigned int entityID) override;

  public:
    explicit MapSystem(MessageBus *messageBus, Evaluator evaluator);
};
}; // namespace JamJar

#endif
