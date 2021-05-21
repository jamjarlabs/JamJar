#include "render/material.hpp"

const JamJar::Color DEFAULT_COLOR(0.54, 0, 0.54, 1);

JamJar::Material::Material(Color color, const Texture &texture) : color(color), texture(texture) {}

JamJar::Material::Material(Color color) : color(color), texture(std::nullopt) {}

JamJar::Material::Material() : Material(DEFAULT_COLOR) {}
