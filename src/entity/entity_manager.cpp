#include "entity/entity_manager.hpp"
#include "component/component.hpp"
#include "component/component_manager.hpp"
#include "entity/entity.hpp"
#include "message/message_payload.hpp"
#include "system/stateful_system.hpp"

JamJar::EntityManager::EntityManager(JamJar::MessageBus *messageBus) : m_messageBus(messageBus) {
    this->m_messageBus->Subscribe(this, JamJar::Component::MESSAGE_ADD);
    this->m_messageBus->Subscribe(this, JamJar::Component::MESSAGE_REMOVE);
    this->m_messageBus->Subscribe(this, JamJar::Entity::MESSAGE_CREATE);
    this->m_messageBus->Subscribe(this, JamJar::Entity::MESSAGE_DESTROY);
}

void JamJar::EntityManager::OnMessage(JamJar::Message *message) {
    switch (message->type) {
    case JamJar::Entity::MESSAGE_CREATE: {
        auto *createMessage = static_cast<JamJar::MessagePayload<std::unique_ptr<JamJar::Entity>> *>(message);
        this->createEntity(std::move(createMessage->payload));
        break;
    }
    case JamJar::Entity::MESSAGE_DESTROY: {
        auto *destroyMessage = static_cast<JamJar::MessagePayload<JamJar::Entity *> *>(message);
        this->destroyEntity(destroyMessage->payload);
        break;
    }
    case JamJar::Component::MESSAGE_ADD: {
        auto *addMessage =
            static_cast<JamJar::MessagePayload<std::unique_ptr<JamJar::AddComponentPayloadPair>> *>(message);
        this->addComponent(addMessage->payload->first, std::move(addMessage->payload->second));
        break;
    }
    case JamJar::Component::MESSAGE_REMOVE: {
        auto *removeMessage =
            static_cast<JamJar::MessagePayload<std::unique_ptr<JamJar::RemoveComponentPayloadPair>> *>(message);
        this->removeComponent(removeMessage->payload->first, removeMessage->payload->second);
        break;
    }
    }
}

void JamJar::EntityManager::createEntity(std::unique_ptr<JamJar::Entity> entity) {
    this->m_entities.insert({entity->id, std::move(entity)});
}

void JamJar::EntityManager::registerEntity(JamJar::Entity *entity) {
    auto components = std::vector<JamJar::Component *>(this->getComponents(entity));
    auto pair = std::make_unique<JamJar::RegisterEntityPayloadPair>(entity, components);
    auto msg = std::make_unique<JamJar::MessagePayload<std::unique_ptr<JamJar::RegisterEntityPayloadPair>>>(
        JamJar::EntityManager::MESSAGE_REGISTER, std::move(pair));
    this->m_messageBus->Publish(std::move(msg));
}

void JamJar::EntityManager::destroyEntity(JamJar::Entity *entity) {
    for (const auto &componentManager : this->m_componentManagers) {
        componentManager->Remove(entity->id);
    }
    auto msg =
        std::make_unique<JamJar::MessagePayload<unsigned int>>(JamJar::EntityManager::MESSAGE_DEREGISTER, entity->id);
    this->m_messageBus->Publish(std::move(msg));
    this->m_entities.erase(entity->id);
}

void JamJar::EntityManager::removeComponent(JamJar::Entity *entity, uint32_t key) {
    auto *componentManager = this->getComponentManager(key);
    if (componentManager == nullptr) {
        return;
    }
    componentManager->Remove(entity->id);
    this->registerEntity(entity);
}

void JamJar::EntityManager::addComponent(JamJar::Entity *entity, std::unique_ptr<Component> component) {
    auto *componentManager = this->getComponentManager(component->key);
    if (componentManager == nullptr) {
        auto newComponentManager = std::make_unique<JamJar::ComponentManager>(component->key);
        componentManager = newComponentManager.get();
        this->m_componentManagers.push_back(std::move(newComponentManager));
    }
    componentManager->Add(entity->id, std::move(component));
    this->registerEntity(entity);
}

std::vector<JamJar::Component *> JamJar::EntityManager::getComponents(JamJar::Entity *entity) {
    auto components = std::vector<JamJar::Component *>();
    for (const auto &componentManager : this->m_componentManagers) {
        auto *component = componentManager->Get(entity->id);
        if (component != nullptr) {
            components.push_back(component);
        }
    }
    return components;
}

JamJar::ComponentManager *JamJar::EntityManager::getComponentManager(uint32_t key) {
    for (const auto &componentManager : this->m_componentManagers) {
        if (componentManager->key == key) {
            return componentManager.get();
        }
    }
    return nullptr;
}
