#ifndef ROTATOR_HPP
#define ROTATOR_HPP

#include "game.hpp"
#include "message/message_bus.hpp"
#include <memory>

class Rotator : public JamJar::Game {
  protected:
    virtual void OnStart() override;

  public:
    Rotator(JamJar::MessageBus *messageBus);
};

#endif
