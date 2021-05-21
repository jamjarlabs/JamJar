#include "standard/2d/sprite/sprite.hpp"

#include <utility>

JamJar::Standard::_2D::Sprite::Sprite(Material material) : Component(KEY), material(std::move(material)) {}
