#include "standard/2d/sprite/sprite.hpp"
#include "standard/2d/sprite/sprite_system.hpp"

JamJar::Standard::_2D::Sprite::Sprite(Material material) : Component(KEY), material(std::move(material)) {
    if (!this->material.shaders.has_value()) {
        this->material.shaders.emplace(JamJar::ShaderGroup({
            .vertexShader = SpriteSystem::DEFAULT_SPRITE_VERTEX_SHADER_NAME,
            .fragmentShader = SpriteSystem::DEFAULT_SPRITE_FRAGMENT_SHADER_NAME,
        }));
    }
}
