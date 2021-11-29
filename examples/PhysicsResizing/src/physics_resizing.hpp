#ifndef SIMPLE_GAME_HPP
#define SIMPLE_GAME_HPP

#include "game.hpp"
#include "message/message_bus.hpp"
#include <memory>

class PhysicsResizing : public JamJar::Game {
  protected:
    virtual void OnStart() override;

  public:
    PhysicsResizing(JamJar::MessageBus *messageBus);
};

#endif
