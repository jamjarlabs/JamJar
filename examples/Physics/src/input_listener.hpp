#ifndef INPUT_LISTENER_HPP
#define INPUT_LISTENER_HPP

#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "system/system.hpp"

class InputListener : public JamJar::System {
  public:
    explicit InputListener(JamJar::MessageBus *messageBus);
    void OnMessage(JamJar::Message *message) override;
};

#endif
