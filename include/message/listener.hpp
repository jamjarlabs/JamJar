#ifndef MESSAGE_LISTENER_HPP
#define MESSAGE_LISTENER_HPP

#include "message/message.hpp"
#include <cstdint>

namespace JamJar {
class Listener {
  private:
    static uint32_t ID;

  public:
    Listener();
    uint32_t listenerID;
    virtual void OnMessage(Message *message) = 0;
    virtual ~Listener() = default;
};
}; // namespace JamJar

#endif
