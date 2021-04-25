#ifndef COMPONENT_COMPONENT_MANAGER_HPP
#define COMPONENT_COMPONENT_MANAGER_HPP

#include "component/component.hpp"
#include <map>
#include <memory>

namespace JamJar {
class ComponentManager {
  private:
    std::map<unsigned int, std::unique_ptr<Component>> m_components;

  public:
    uint32_t m_key;

    explicit ComponentManager(uint32_t key);

    Component *Get(unsigned int id);
    void Add(unsigned int id, std::unique_ptr<Component> component);
    void Remove(unsigned int id);
};
}; // namespace JamJar

#endif
