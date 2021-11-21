#include "system/map_system.hpp"
#include "component/component.hpp"
#include "entity/entity.hpp"
#include "message/message_bus.hpp"
#include "system/evaluator.hpp"
#include "system/system.hpp"

JamJar::MapSystem::MapSystem(MessageBus *messageBus, Evaluator evaluator)
    : StatefulSystem(messageBus), m_evaluator(std::move(evaluator)) {}

bool JamJar::MapSystem::registerEntity(Entity *entity, std::vector<Component *> components) {
    if (!this->m_evaluator(entity, components)) {
        this->removeEntity(entity->id);
        return false;
    }

    if (this->entities.count(entity->id) != 0) {
        // If the entity is already registered, don't register it again
        return false;
    }

    this->entities.insert({entity->id, JamJar::SystemEntity(entity, components)});
    return true;
}

void JamJar::MapSystem::removeEntity(unsigned int entityID) { this->entities.erase(entityID); }
