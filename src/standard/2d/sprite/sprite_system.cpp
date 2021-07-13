#include "standard/2d/sprite/sprite_system.hpp"
#include "game.hpp"
#include "message/message_payload.hpp"
#include "standard/2d/render/render_system.hpp"
#include "standard/2d/render/renderable.hpp"
#include "standard/2d/sprite/sprite.hpp"
#include "standard/2d/transform/transform.hpp"
#include <array>
#include <memory>

const std::string JamJar::Standard::_2D::SpriteSystem::DEFAULT_SPRITE_VERTEX_SHADER_NAME =
    "jamjar_default_sprite_vertex";
const std::string JamJar::Standard::_2D::SpriteSystem::DEFAULT_SPRITE_FRAGMENT_SHADER_NAME =
    "jamjar_default_sprite_fragment";

JamJar::Standard::_2D::SpriteSystem::SpriteSystem(MessageBus *messageBus)
    : MapSystem(messageBus, JamJar::Standard::_2D::SpriteSystem::evaluator) {
    this->messageBus->Subscribe(this, JamJar::Game::MESSAGE_PRE_RENDER);
#ifdef __EMSCRIPTEN__
    this->loadWebgl2Shaders();
#endif
}

#ifdef __EMSCRIPTEN__
#include "standard/2d/sprite/webgl2_default_sprite_shaders.hpp"
#include "standard/2d/webgl2/webgl2_system.hpp"
void JamJar::Standard::_2D::SpriteSystem::loadWebgl2Shaders() {
    // Load WebGL2 fragment shader
    auto fragShader =
        std::make_unique<WebGL2DefaultSpriteFragmentShader>(JamJar::Standard::_2D::WebGL2DefaultSpriteFragmentShader());
    this->messageBus->Publish(
        std::make_unique<JamJar::MessagePayload<std::unique_ptr<WebGL2DefaultSpriteFragmentShader>>>(
            JamJar::Standard::_2D::WebGL2System::MESSAGE_LOAD_SHADER, std::move(fragShader)));
    // Load WebGL2 vertex shader
    auto vertShader =
        std::make_unique<WebGL2DefaultSpriteVertexShader>(JamJar::Standard::_2D::WebGL2DefaultSpriteVertexShader());
    this->messageBus->Publish(
        std::make_unique<JamJar::MessagePayload<std::unique_ptr<WebGL2DefaultSpriteVertexShader>>>(
            JamJar::Standard::_2D::WebGL2System::MESSAGE_LOAD_SHADER, std::move(vertShader)));
}
#endif

const std::vector<float> QUAD_POINTS = {0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, -0.5};

bool JamJar::Standard::_2D::SpriteSystem::evaluator(Entity *entity,
                                                    const std::vector<JamJar::Component *> &components) {
    bool hasSprite = false;
    bool hasTransform = false;
    for (const auto &component : components) {
        if (component->key == JamJar::Standard::_2D::Sprite::KEY) {
            hasSprite = true;
        }
        if (component->key == JamJar::Standard::_2D::Transform::KEY) {
            hasTransform = true;
        }
        if (hasSprite && hasTransform) {
            return true;
        }
    }
    return false;
}

void JamJar::Standard::_2D::SpriteSystem::OnMessage(JamJar::Message *message) {
    MapSystem::OnMessage(message);
    switch (message->type) {
    case JamJar::Game::MESSAGE_PRE_RENDER: {
        auto *renderMessage = static_cast<JamJar::MessagePayload<float> *>(message);
        this->preRender(renderMessage->payload);
        break;
    }
    }
}

void JamJar::Standard::_2D::SpriteSystem::preRender(float alpha) {
    std::vector<Renderable> renderables;
    for (const auto &entityPair : this->entities) {
        auto entity = entityPair.second;
        auto *transform =
            static_cast<JamJar::Standard::_2D::Transform *>(entity.Get(JamJar::Standard::_2D::Transform::KEY));
        auto *sprite = static_cast<JamJar::Standard::_2D::Sprite *>(entity.Get(JamJar::Standard::_2D::Sprite::KEY));

        auto quad = AABB(Vector2D(1, 1));
        quad.dimensions += transform->scale;
        quad.center += transform->position;

        renderables.push_back({.vertices = QUAD_POINTS,
                               .modelMatrix = transform->InterpolatedMatrix4D(alpha),
                               .material = sprite->material,
                               .drawMode = JamJar::Standard::_2D::DrawMode::TRIANGLES});
    }
    auto msg = std::make_unique<JamJar::MessagePayload<std::vector<Renderable>>>(
        JamJar::Standard::_2D::RenderSystem::MESSAGE_LOAD_RENDERABLES, renderables);
    this->messageBus->Publish(std::move(msg));
}
