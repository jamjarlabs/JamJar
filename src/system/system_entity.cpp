#include "system/system_entity.hpp"
#include "component/component.hpp"
#include "entity/entity.hpp"

JamJar::SystemEntity::SystemEntity(Entity *entity, const std::vector<JamJar::Component *> &components)
    : entity(entity) {
    for (const auto &component : components) {
        this->m_components[component->key] = component;
    }
}

JamJar::Component *JamJar::SystemEntity::Get(uint32_t key) { return this->m_components[key]; }

void JamJar::SystemEntity::Remove(uint32_t key) {
    this->m_components.erase(key);
    this->entity->Remove(key);
}

void JamJar::SystemEntity::Add(std::unique_ptr<JamJar::Component> component) {
    this->m_components[component->key] = component.get();
    this->entity->Add(std::move(component));
}

void JamJar::SystemEntity::Destroy() const { this->entity->Destroy(); }
