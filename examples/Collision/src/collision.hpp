#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "game.hpp"
#include "message/message_bus.hpp"
#include <memory>

class Collision : public JamJar::Game {
  protected:
    virtual void OnStart() override;

  public:
    Collision(JamJar::MessageBus *messageBus);
};

#endif
