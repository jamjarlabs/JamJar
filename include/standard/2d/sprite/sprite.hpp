#ifndef STANDARD_2D_SPRITE_SPRITE_HPP
#define STANDARD_2D_SPRITE_SPRITE_HPP

#include "component/component.hpp"
#include "geometry/aabb.hpp"
#include "render/material.hpp"

namespace JamJar::Standard::_2D {
class Sprite : public Component {
  public:
    constexpr static uint32_t KEY = JamJar::hash("jamjar_2d_sprite");
    explicit Sprite(Material material);
    Material material;
};
}; // namespace JamJar::Standard::_2D

#endif
