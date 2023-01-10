#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "game.hpp"
#include "message/message_bus.hpp"
#include <memory>

class Cursor : public JamJar::Game {
  protected:
    virtual void OnStart() override;

  public:
    Cursor(JamJar::MessageBus *messageBus);
};

#endif
