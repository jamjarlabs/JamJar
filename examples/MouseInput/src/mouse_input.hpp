#ifndef MOUSE_INPUT_HPP
#define MOUSE_INPUT_HPP

#include "game.hpp"
#include "message/message_bus.hpp"
#include <memory>

class MouseInput : public JamJar::Game {
  protected:
    virtual void OnStart() override;

  public:
    MouseInput(JamJar::MessageBus *messageBus);
};

#endif
