#ifndef BOX_HPP
#define BOX_HPP

#include "component/component.hpp"

class Box : public JamJar::Component {
  public:
    constexpr static uint32_t KEY = JamJar::hash("box");
    explicit Box();
};

#endif
