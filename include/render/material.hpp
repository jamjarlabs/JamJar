#ifndef RENDER_MATERIAL_HPP
#define RENDER_MATERIAL_HPP

#include "render/color.hpp"
#include "render/texture.hpp"
#include <optional>
#include <string>

namespace JamJar {
struct ShaderGroup {
    std::string vertexShader;
    std::string fragmentShader;
};
class Material {
  public:
    Material();
    explicit Material(Color color);
    explicit Material(Color color, const Texture &texture);
    explicit Material(Color color, const ShaderGroup &shaders);
    explicit Material(Color color, const ShaderGroup &shaders, const Texture &texture);
    Color color;
    std::optional<Texture> texture;
    std::optional<ShaderGroup> shaders;
};
}; // namespace JamJar

#endif
