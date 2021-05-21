#ifndef MESSAGE_MESSAGE_PAYLOAD_HPP
#define MESSAGE_MESSAGE_PAYLOAD_HPP

#include "message/message.hpp"
#include <cstdint>
#include <memory>

namespace JamJar {
template <typename T> class MessagePayload : public Message {
  public:
    MessagePayload<T>(uint32_t type, T payload) : Message(type), payload(payload) {}
    T payload;
};

template <typename T> class MessagePayload<std::unique_ptr<T>> : public Message {
  public:
    MessagePayload<std::unique_ptr<T>>(uint32_t type, std::unique_ptr<T> payload)
        : Message(type), payload(std::move(payload)) {}
    std::unique_ptr<T> payload;
};
}; // namespace JamJar

#endif
