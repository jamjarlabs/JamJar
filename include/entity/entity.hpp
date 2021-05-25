#ifndef ENTITY_ENTITY_HPP
#define ENTITY_ENTITY_HPP

#include "component/component.hpp"
#include "hash.hpp"
#include "message/message_bus.hpp"
#include <memory>
#include <utility>

namespace JamJar {
class Entity {
  private:
    static unsigned int ID;

    MessageBus *m_messageBus;

  public:
    constexpr static uint32_t MESSAGE_CREATE = JamJar::hash("jamjar_entity_create");
    constexpr static uint32_t MESSAGE_DESTROY = JamJar::hash("jamjar_entity_destroy");

    unsigned int id;

    explicit Entity(MessageBus *messageBus);

    void Add(std::unique_ptr<Component> component);
    void Remove(uint32_t key);
    void Destroy();
};

using AddComponentPayloadPair = std::pair<JamJar::Entity *, std::unique_ptr<JamJar::Component>>;
using RemoveComponentPayloadPair = std::pair<JamJar::Entity *, uint32_t>;
}; // namespace JamJar

#endif
