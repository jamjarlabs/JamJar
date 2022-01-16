#ifndef SYSTEM_BUCKET_MAP_SYSTEM_HPP
#define SYSTEM_BUCKET_MAP_SYSTEM_HPP

#include "component/component.hpp"
#include "entity/entity.hpp"
#include "system/stateful_system.hpp"
#include "system/system_entity.hpp"
#include <map>
#include <optional>
#include <vector>
#include <functional>

namespace JamJar {
class BucketMapSystem : public StatefulSystem {
  public:
    using Evaluator = std::function<std::optional<uint32_t>(Entity *, const std::vector<JamJar::Component *> &)>;
    explicit BucketMapSystem(MessageBus *messageBus, Evaluator evaluator);

  protected:
    std::map<uint32_t, std::map<unsigned int, SystemEntity>> entities{};
    bool registerEntity(Entity *entity, std::vector<Component *> components) override;
    void removeEntity(unsigned int entityID) override;

  private:
    bool isEntityRegistered(unsigned int entityID);
    Evaluator evaluator{};
};
}; // namespace JamJar

#endif
