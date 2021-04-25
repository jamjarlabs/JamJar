#ifndef MESSAGE_SUBSCRIBER_HPP
#define MESSAGE_SUBSCRIBER_HPP

#include "message/message.hpp"
#include <cstdint>

namespace JamJar {
class Subscriber {
  private:
    static uint32_t SubscriberID;

  public:
    Subscriber();
    uint32_t m_subscriberID;
    virtual void OnMessage(Message *message) = 0;
};
}; // namespace JamJar

#endif
