#include "system/vector_system.hpp"
#include "component/component.hpp"
#include "entity/entity.hpp"
#include "message/message_bus.hpp"
#include "system/system.hpp"

JamJar::VectorSystem::VectorSystem(MessageBus *messageBus, Evaluator evaluator)
    : StatefulSystem(messageBus), evaluator(std::move(evaluator)) {}

bool JamJar::VectorSystem::registerEntity(Entity *entity, std::vector<Component *> components) {
    if (!this->evaluator(entity, components)) {
        this->removeEntity(entity->id);
        return false;
    }

    if (this->isEntityRegistered(entity->id)) {
        // If the entity is already registered, don't register it again
        return false;
    }

    this->entities.push_back(JamJar::SystemEntity(entity, components));
    return true;
}

void JamJar::VectorSystem::removeEntity(unsigned int entityID) {
    for (std::vector<SystemEntity>::iterator iter = this->entities.begin(); iter != this->entities.end();) {
        if (iter->entity->id == entityID) {
            iter = this->entities.erase(iter);
        } else {
            ++iter;
        }
    }
}

bool JamJar::VectorSystem::isEntityRegistered(unsigned int entityID) {
    for (const auto &entity : this->entities) {
        if (entity.entity->id == entityID) {
            return true;
        }
    }
    return false;
}
