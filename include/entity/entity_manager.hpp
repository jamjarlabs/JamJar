#ifndef ENTITY_ENTITY_MANAGER_HPP
#define ENTITY_ENTITY_MANAGER_HPP

#include "component/component_manager.hpp"
#include "entity/entity.hpp"
#include "message/message_bus.hpp"
#include "message/listener.hpp"
#include <memory>
#include <unordered_map>
#include <vector>

namespace JamJar {
using RegisterEntityPayloadPair = std::pair<JamJar::Entity *, std::vector<JamJar::Component *>>;
class EntityManager : public Listener {
  private:
    std::vector<std::unique_ptr<ComponentManager>> m_componentManagers;
    std::unordered_map<unsigned int, std::unique_ptr<JamJar::Entity>> m_entities;
    MessageBus *m_messageBus;
    void createEntity(std::unique_ptr<JamJar::Entity> entity);
    void registerEntity(JamJar::Entity *entity);
    void destroyEntity(JamJar::Entity *entity);
    void removeComponent(JamJar::Entity *entity, uint32_t key);
    void addComponent(JamJar::Entity *entity, std::unique_ptr<Component> component);
    ComponentManager *getComponentManager(uint32_t key);
    std::vector<Component *> getComponents(Entity *entity);

  public:
    explicit EntityManager(MessageBus *messageBus);
    void OnMessage(Message *message) override;

    constexpr static uint32_t MESSAGE_REGISTER = hash("jamjar_entity_manager_register");
    constexpr static uint32_t MESSAGE_DEREGISTER = hash("jamjar_entity_manager_deregister");
};
}; // namespace JamJar

#endif
