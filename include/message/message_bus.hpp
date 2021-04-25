#ifndef MESSAGE_MESSAGE_BUS_HPP
#define MESSAGE_MESSAGE_BUS_HPP

#include "message/message.hpp"
#include "message/subscriber.hpp"
#include <map>
#include <memory>
#include <queue>

namespace JamJar {
class MessageBus {
  private:
    std::map<uint32_t, std::vector<Subscriber *>> m_subscribers;
    std::queue<std::unique_ptr<Message>> m_messages;

  public:
    MessageBus();
    void Dispatch();
    void Publish(std::unique_ptr<Message> message);
    void Subscribe(Subscriber *subscriber, uint32_t type);
};
}; // namespace JamJar

#endif
