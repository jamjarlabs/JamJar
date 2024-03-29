#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include "game.hpp"
#include "message/message_bus.hpp"
#include <memory>

class Primitives : public JamJar::Game {
  protected:
    virtual void OnStart() override;

  public:
    Primitives(JamJar::MessageBus *messageBus);
};

#endif
