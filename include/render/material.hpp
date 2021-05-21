#ifndef RENDER_MATERIAL_HPP
#define RENDER_MATERIAL_HPP

#include "render/color.hpp"
#include "render/texture.hpp"
#include <optional>

namespace JamJar {
class Material {
  public:
    Material();
    explicit Material(Color color);
    explicit Material(Color color, const Texture &texture);
    Color color;
    std::optional<Texture> texture;
};
}; // namespace JamJar

#endif
