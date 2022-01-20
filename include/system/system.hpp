#ifndef SYSTEM_SYSTEM_HPP
#define SYSTEM_SYSTEM_HPP

#include "hash.hpp"
#include "message/listener.hpp"
#include "message/message_bus.hpp"

namespace JamJar {
class System : public Listener {
  protected:
    MessageBus *messageBus;
    virtual void update(float deltaTime){};

  public:
    constexpr static uint32_t MESSAGE_UPDATE = hash("jamjar_update");

    explicit System(MessageBus *messageBus);
    void OnMessage(Message *message) override;
    ~System() override = default;
};
}; // namespace JamJar

#endif
