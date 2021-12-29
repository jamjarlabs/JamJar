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
    Entity *entity;
    explicit SystemEntity(Entity *entity, const std::vector<JamJar::Component *> &components);
    bool Has(uint32_t key);

    template <class T, typename = std::enable_if_t<std::is_base_of_v<JamJar::Component, T>>> T *Get() {
        return static_cast<T *>(this->m_components[T::KEY]);
    };

    void Remove(uint32_t key);
    void Add(JamJar::Component *component);
    void Destroy() const;
};
}; // namespace JamJar

#endif
