#include "system/bucket_system.hpp"
#include "component/component.hpp"
#include "entity/entity.hpp"
#include "message/message_bus.hpp"
#include "system/system.hpp"

JamJar::BucketSystem::BucketSystem(MessageBus *messageBus, Evaluator evaluator)
    : StatefulSystem(messageBus), evaluator(std::move(evaluator)) {}

bool JamJar::BucketSystem::registerEntity(Entity *entity, std::vector<Component *> components) {
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
        this->entities.insert({result.value(), std::vector<SystemEntity>{JamJar::SystemEntity(entity, components)}});
    } else {
        this->entities[result.value()].push_back(JamJar::SystemEntity(entity, components));
    }
    return true;
}

void JamJar::BucketSystem::removeEntity(unsigned int entityID) {
    for (auto &[key, bucket] : this->entities) {
        for (std::vector<SystemEntity>::iterator iter = bucket.begin(); iter != bucket.end();) {
            if (iter->entity->id == entityID) {
                iter = bucket.erase(iter);
            } else {
                ++iter;
            }
        }
    }
}

bool JamJar::BucketSystem::isEntityRegistered(unsigned int entityID) {
    for (const auto &[key, bucket] : this->entities) {
        for (const auto &entity : bucket) {
            if (entity.entity->id == entityID) {
                return true;
            }
        }
    }
    return false;
}
