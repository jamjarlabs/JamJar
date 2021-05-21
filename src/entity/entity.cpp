#include "entity/entity.hpp"
#include "component/component.hpp"
#include "hash.hpp"
#include "message/message_payload.hpp"

unsigned int JamJar::Entity::ID = 0;

JamJar::Entity::Entity(MessageBus *messageBus) : m_messageBus(messageBus), id(++ID) {
    // This is maybe a bit weird/bad, when an entity is created it will automatically send out a message with a
    // unique_ptr to register the entity as being created, an entity manager can then pick this up to handle memory
    // management of the entity
    auto msg = std::make_unique<JamJar::MessagePayload<std::unique_ptr<JamJar::Entity>>>(
        JamJar::Entity::MESSAGE_CREATE, std::make_unique<JamJar::Entity>(*this));
    this->m_messageBus->Publish(std::move(msg));
}

void JamJar::Entity::Add(std::unique_ptr<Component> component) {
    // Publish a message with the current entity and the component being added
    auto pair = std::make_unique<JamJar::AddComponentPayloadPair>(this, std::move(component));
    auto msg = std::make_unique<JamJar::MessagePayload<std::unique_ptr<JamJar::AddComponentPayloadPair>>>(
        JamJar::Component::MESSAGE_ADD, std::move(pair));
    this->m_messageBus->Publish(std::move(msg));
}

void JamJar::Entity::Remove(uint32_t key) {
    // Publish a message with the current entity and the key of the component to remove
    auto pair = std::make_unique<JamJar::RemoveComponentPayloadPair>(this, key);
    auto msg = std::make_unique<JamJar::MessagePayload<std::unique_ptr<JamJar::RemoveComponentPayloadPair>>>(
        JamJar::Component::MESSAGE_REMOVE, std::move(pair));
    this->m_messageBus->Publish(std::move(msg));
}

void JamJar::Entity::Destroy() {
    // Publish a message with the current entity as the one being destroyed
    auto msg = std::make_unique<JamJar::MessagePayload<JamJar::Entity &>>(MESSAGE_DESTROY, *this);
    this->m_messageBus->Publish(std::move(msg));
}
