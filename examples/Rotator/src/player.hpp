#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "component/component.hpp"

class Player : public JamJar::Component {
  public:
    constexpr static uint32_t KEY = JamJar::hash("player");
    explicit Player();
};

#endif
