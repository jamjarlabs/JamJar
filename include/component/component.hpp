#ifndef COMPONENT_COMPONENT_HPP
#define COMPONENT_COMPONENT_HPP

#include "hash.hpp"

namespace JamJar {
class Component {
  public:
    explicit Component(uint32_t key);

    constexpr static uint32_t MESSAGE_ADD = hash("jamjar_component_add");
    constexpr static uint32_t MESSAGE_REMOVE = hash("jamjar_component_remove");

    uint32_t key;
};
}; // namespace JamJar

#endif
