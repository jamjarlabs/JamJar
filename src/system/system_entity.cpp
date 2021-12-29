#include "system/system_entity.hpp"
#include "component/component.hpp"
#include "entity/entity.hpp"

JamJar::SystemEntity::SystemEntity(Entity *entity, const std::vector<JamJar::Component *> &components)
    : entity(entity) {
    for (const auto &component : components) {
        this->m_components[component->key] = component;
    }
}

bool JamJar::SystemEntity::Has(uint32_t key) { return this->m_components.count(key) != 0; }

void JamJar::SystemEntity::Remove(uint32_t key) {
    this->m_components.erase(key);
    this->entity->Remove(key);
}

void JamJar::SystemEntity::Add(JamJar::Component *component) {
    this->m_components[component->key] = component;
    this->entity->Add(component);
}

void JamJar::SystemEntity::Destroy() const { this->entity->Destroy(); }
