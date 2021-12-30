#include "system/single_entity_system.hpp"
#include "component/component.hpp"
#include "entity/entity.hpp"
#include "message/message_bus.hpp"
#include "system/system.hpp"
#include <stdexcept>

JamJar::SingleEntitySystem::SingleEntitySystem(MessageBus *messageBus, Evaluator evaluator, bool exceptionOnOverwrite)
    : StatefulSystem(messageBus), evaluator(std::move(evaluator)), exceptionOnOverwrite(exceptionOnOverwrite) {}

JamJar::SingleEntitySystem::SingleEntitySystem(MessageBus *messageBus, Evaluator evaluator)
    : SingleEntitySystem(messageBus, evaluator, false) {}

bool JamJar::SingleEntitySystem::registerEntity(Entity *entity, std::vector<Component *> components) {
    if (!this->evaluator(entity, components)) {
        this->removeEntity(entity->id);
        return false;
    }

    if (!this->entity.has_value()) {
        // No value, set value
        this->entity.emplace(JamJar::SystemEntity(entity, components));
        return true;
    }

    if (this->entity.value().entity->id != entity->id && exceptionOnOverwrite) {
        // Raise exception
        throw SingleEntitySystemOverwriteException();
    }

    this->entity.emplace(JamJar::SystemEntity(entity, components));
    return true;
}

void JamJar::SingleEntitySystem::removeEntity(unsigned int entityID) {
    if (!this->entity.has_value()) {
        return;
    }

    if (this->entity.value().entity->id != entityID) {
        return;
    }

    this->entity.reset();
}
