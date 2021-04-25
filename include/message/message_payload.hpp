#ifndef MESSAGE_MESSAGE_PAYLOAD_HPP
#define MESSAGE_MESSAGE_PAYLOAD_HPP

#include "message/message.hpp"
#include <cstdint>
#include <memory>

namespace JamJar {
template <typename T> class MessagePayload : public Message {
  public:
    MessagePayload<T>(uint32_t type, T payload) : Message(type), m_payload(payload) {}
    T m_payload;
};

template <typename T> class MessagePayload<std::unique_ptr<T>> : public Message {
  public:
    MessagePayload<std::unique_ptr<T>>(uint32_t type, std::unique_ptr<T> payload)
        : Message(type), m_payload(std::move(payload)) {}
    std::unique_ptr<T> m_payload;
};
}; // namespace JamJar

#endif
