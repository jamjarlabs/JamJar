#ifndef SYSTEM_SINGLE_ENTITY_SYSTEM_HPP
#define SYSTEM_SINGLE_ENTITY_SYSTEM_HPP

#include "component/component.hpp"
#include "entity/entity.hpp"
#include "system/stateful_system.hpp"
#include "system/system_entity.hpp"
#include <functional>
#include <vector>

namespace JamJar {

struct SingleEntitySystemOverwriteException : public std::exception {
    const char *what() const throw() {
        return "Overwrite attempted of single entity system with exceptionOnOverwrite=true";
    }
};

class SingleEntitySystem : public StatefulSystem {
  public:
    using Evaluator = std::function<bool(Entity *, const std::vector<JamJar::Component *> &)>;
    explicit SingleEntitySystem(MessageBus *messageBus, Evaluator evaluator, bool exceptionOnOverwrite);
    explicit SingleEntitySystem(MessageBus *messageBus, Evaluator evaluator);

  protected:
    std::optional<SystemEntity> entity;
    bool registerEntity(Entity *entity, std::vector<Component *> components) override;
    void removeEntity(unsigned int entityID) override;

  private:
    bool exceptionOnOverwrite;
    Evaluator evaluator{};
};
}; // namespace JamJar

#endif
