#ifndef MESSAGE_MESSAGE_HPP
#define MESSAGE_MESSAGE_HPP

#include <cstdint>

namespace JamJar {
class Message {
  public:
    explicit Message(uint32_t type) : m_type(type) {}
    uint32_t m_type{};
};
}; // namespace JamJar

#endif
