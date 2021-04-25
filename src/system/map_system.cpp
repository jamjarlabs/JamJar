#include "system/map_system.hpp"
#include "component/component.hpp"
#include "entity/entity.hpp"
#include "message/message_bus.hpp"
#include "system/evaluator.hpp"
#include "system/system.hpp"

JamJar::MapSystem::MapSystem(MessageBus *messageBus, Evaluator evaluator)
    : StatefulSystem(messageBus), m_evaluator(std::move(evaluator)) {}

void JamJar::MapSystem::registerEntity(Entity *entity, std::vector<Component *> components) {

    this->removeEntity(entity->m_id);

    if (!this->m_evaluator(entity, components)) {
        return;
    }

    this->m_entities.insert({entity->m_id, JamJar::SystemEntity(entity, components)});
}

void JamJar::MapSystem::removeEntity(unsigned int entityID) { this->m_entities.erase(entityID); }
