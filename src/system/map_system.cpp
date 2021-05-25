#include "system/map_system.hpp"
#include "component/component.hpp"
#include "entity/entity.hpp"
#include "message/message_bus.hpp"
#include "system/evaluator.hpp"
#include "system/system.hpp"

JamJar::MapSystem::MapSystem(MessageBus *messageBus, Evaluator evaluator)
    : StatefulSystem(messageBus), m_evaluator(std::move(evaluator)) {}

void JamJar::MapSystem::registerEntity(Entity *entity, std::vector<Component *> components) {

    this->removeEntity(entity->id);

    if (!this->m_evaluator(entity, components)) {
        return;
    }

    this->entities.insert({entity->id, JamJar::SystemEntity(entity, components)});
}

void JamJar::MapSystem::removeEntity(unsigned int entityID) { this->entities.erase(entityID); }
