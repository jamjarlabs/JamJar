#ifndef DESTRUCTOR_HPP
#define DESTRUCTOR_HPP

#include "component/component.hpp"

class Destructor : public JamJar::Component {
  public:
    constexpr static uint32_t KEY = JamJar::hash("destructor");
    explicit Destructor();
};

#endif
