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
    explicit Material(JamJar::Color color);
    explicit Material(JamJar::Color color, const JamJar::Texture &texture);
    explicit Material(JamJar::Color color, const JamJar::ShaderGroup &shaders);
    explicit Material(JamJar::Color color, const JamJar::ShaderGroup &shaders, const JamJar::Texture &texture);
    Color color;
    std::optional<JamJar::Texture> texture;
    std::optional<JamJar::ShaderGroup> shaders;
};
}; // namespace JamJar

#endif
