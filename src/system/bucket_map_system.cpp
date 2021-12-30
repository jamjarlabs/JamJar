#include "system/bucket_map_system.hpp"
#include "component/component.hpp"
#include "entity/entity.hpp"
#include "message/message_bus.hpp"
#include "system/system.hpp"

JamJar::BucketMapSystem::BucketMapSystem(MessageBus *messageBus, Evaluator evaluator)
    : StatefulSystem(messageBus), evaluator(std::move(evaluator)) {}

bool JamJar::BucketMapSystem::registerEntity(Entity *entity, std::vector<Component *> components) {
    auto result = this->evaluator(entity, components);
    if (!result.has_value()) {
        this->removeEntity(entity->id);
        return false;
    }

    if (this->isEntityRegistered(entity->id)) {
        // If the entity is already registered, don't register it again
        return false;
    }

    if (this->entities.count(result.value()) == 0) {
        this->entities.insert({result.value(), std::map<unsigned int, SystemEntity>{{entity->id, JamJar::SystemEntity(entity, components)}}});
    } else {
        this->entities[result.value()].insert({entity->id, JamJar::SystemEntity(entity, components)});
    }
    return true;
}

void JamJar::BucketMapSystem::removeEntity(unsigned int entityID) {
    for (auto &[key, bucket] : this->entities) {
        bucket.erase(entityID);
    }
}

bool JamJar::BucketMapSystem::isEntityRegistered(unsigned int entityID) {
    for (const auto &[key, bucket] : this->entities) {
        if (bucket.count(entityID) > 0) {
            return true;
        }
    }
    return false;
}
