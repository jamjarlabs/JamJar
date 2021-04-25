#ifndef SYSTEM_SYSTEM_ENTITY_HPP
#define SYSTEM_SYSTEM_ENTITY_HPP

#include "component/component.hpp"
#include "entity/entity.hpp"
#include <map>

namespace JamJar {
class SystemEntity {
  private:
    std::map<uint32_t, JamJar::Component *> m_components{};

  public:
    Entity *m_entity;
    explicit SystemEntity(Entity *entity, const std::vector<JamJar::Component *> &components);
    JamJar::Component *Get(uint32_t key);
    void Remove(uint32_t key);
    void Add(std::unique_ptr<JamJar::Component> component);
    void Destroy() const;
};
}; // namespace JamJar

#endif
