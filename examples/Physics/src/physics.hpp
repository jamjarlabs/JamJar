#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "game.hpp"
#include "message/message_bus.hpp"
#include <memory>

class Physics : public JamJar::Game {
  protected:
    virtual void OnStart() override;

  public:
    Physics(JamJar::MessageBus *messageBus);
};

#endif
