#include "simple_game.hpp"
#include "entity/entity.hpp"
#include "game.hpp"
#include "geometry/vector_2d.hpp"
#include "hash.hpp"
#include "message/message_payload.hpp"
#include "render/color.hpp"
#include "render/texture.hpp"
#include "standard/2d/camera/camera.hpp"
#include "standard/2d/motion/motion.hpp"
#include "standard/2d/primitive/primitive.hpp"
#include "standard/2d/sprite/sprite.hpp"
#include "standard/2d/transform/transform.hpp"
#include "standard/file_texture/file_texture_request.hpp"
#include "standard/file_texture/file_texture_system.hpp"
#include <memory>

SimpleGame::SimpleGame(JamJar::MessageBus *messageBus) : Game(messageBus) {}

void SimpleGame::OnStart() {

    this->messageBus->Publish(
        std::make_unique<JamJar::MessagePayload<std::unique_ptr<JamJar::Standard::FileTextureRequest>>>(
            JamJar::Standard::FileTextureSystem::MESSAGE_REQUEST_FILE_TEXTURE_LOAD,
            std::unique_ptr<JamJar::Standard::FileTextureRequest>(new JamJar::Standard::FileTextureRequest(
                {.key = JamJar::hash("smiley"),
                 .path = "/assets/texture.png",
                 .properties = JamJar::TextureProperties(
                     {.minFilter = JamJar::TextureFilter::NEAREST, .magFilter = JamJar::TextureFilter::NEAREST})}))));

    auto entity = new JamJar::Entity(messageBus);
    entity->Add(std::move(
        std::make_unique<JamJar::Standard::_2D::Transform>(JamJar::Vector2D(0, 0), JamJar::Vector2D(30, 30))));
    entity->Add(std::move(
        std::make_unique<JamJar::Standard::_2D::Motion>(JamJar::Vector2D(0, 0), JamJar::Vector2D(0, 0), 0.2)));
    entity->Add(std::move(std::make_unique<JamJar::Standard::_2D::Sprite>(
        JamJar::Material(JamJar::Color(1, 1, 1, 1), JamJar::Texture(JamJar::hash("smiley"))))));

    auto camera = new JamJar::Entity(messageBus);
    camera->Add(std::move(std::make_unique<JamJar::Standard::_2D::Transform>()));
    camera->Add(std::move(std::make_unique<JamJar::Standard::_2D::Camera>(JamJar::Color(0, 0, 0))));
}
