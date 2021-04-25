#include "component/component_manager.hpp"
#include "component/component.hpp"

JamJar::ComponentManager::ComponentManager(uint32_t key) : m_key(key) {}

JamJar::Component *JamJar::ComponentManager::Get(unsigned int id) { return this->m_components[id].get(); }

void JamJar::ComponentManager::Add(unsigned int id, std::unique_ptr<Component> component) {
    this->m_components[id] = std::move(component);
}

void JamJar::ComponentManager::Remove(unsigned int id) { this->m_components.erase(id); }
