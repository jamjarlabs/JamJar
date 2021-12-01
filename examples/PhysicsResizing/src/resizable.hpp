#ifndef RESIZABLE_HPP
#define RESIZABLE_HPP

#include "component/component.hpp"

class Resizable : public JamJar::Component {
  public:
    constexpr static uint32_t KEY = JamJar::hash("resizable");
    explicit Resizable(float min, float max);
    float min;
    float max;
    bool increasing;
};

#endif
