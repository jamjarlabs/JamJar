#ifndef SYSTEM_BUCKET_SYSTEM_HPP
#define SYSTEM_BUCKET_SYSTEM_HPP

#include "component/component.hpp"
#include "entity/entity.hpp"
#include "system/stateful_system.hpp"
#include "system/system_entity.hpp"
#include <map>
#include <optional>
#include <vector>

namespace JamJar {
class BucketSystem : public StatefulSystem {
  public:
    using Evaluator = std::function<std::optional<uint32_t>(Entity *, const std::vector<JamJar::Component *> &)>;
    explicit BucketSystem(MessageBus *messageBus, Evaluator evaluator);

  protected:
    std::map<uint32_t, std::vector<SystemEntity>> entities{};
    bool registerEntity(Entity *entity, std::vector<Component *> components) override;
    void removeEntity(unsigned int entityID) override;

  private:
    bool isEntityRegistered(unsigned int entityID);
    Evaluator evaluator{};
};
}; // namespace JamJar

#endif
