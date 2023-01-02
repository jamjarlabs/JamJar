#include "simple_game.hpp"
#include "entity/entity.hpp"
#include "game.hpp"
#include "geometry/vector_2d.hpp"
#include "hash.hpp"
#include "message/message_payload.hpp"
#include "render/color.hpp"
#include "render/texture.hpp"
#include "standard/2d/camera/camera.hpp"
#include "standard/2d/sprite/sprite.hpp"
#include "standard/2d/transform/transform.hpp"
#include "standard/file_texture/file_texture_request.hpp"
#include "standard/file_texture/file_texture_system.hpp"
#include <memory>

using JamJar::Color;
using JamJar::Entity;
using JamJar::Material;
using JamJar::MessagePayload;
using JamJar::Texture;
using JamJar::TextureFilter;
using JamJar::TextureProperties;
using JamJar::Vector2D;
using JamJar::Standard::FileTextureRequest;
using JamJar::Standard::FileTextureSystem;
using JamJar::Standard::_2D::Camera;
using JamJar::Standard::_2D::Sprite;
using JamJar::Standard::_2D::Transform;

SimpleGame::SimpleGame(JamJar::MessageBus *messageBus) : Game(messageBus) {}

void SimpleGame::OnStart() {

    this->messageBus->Publish(new MessagePayload<std::unique_ptr<FileTextureRequest>>(
        FileTextureSystem::MESSAGE_REQUEST_FILE_TEXTURE_LOAD,
        std::unique_ptr<FileTextureRequest>(new FileTextureRequest(
            {.key = JamJar::hash("smiley"),
             .path = "/assets/texture.png",
             .properties =
                 TextureProperties({.minFilter = TextureFilter::NEAREST, .magFilter = TextureFilter::NEAREST})}))));

    auto a = new Entity(messageBus);
    a->Add(new Transform(Vector2D(-30, 0), Vector2D(30, 30)));
    a->Add(new Sprite(JamJar::Material(JamJar::Color(0, 1, 0, 1), Texture(JamJar::hash("smiley")))));

    auto b = new Entity(messageBus);
    b->Add(new Transform(Vector2D(30, 0), Vector2D(30, 30)));
    b->Add(new Sprite(JamJar::Material(JamJar::Color(1, 0, 1, 1), Texture(JamJar::hash("smiley")))));

    auto camera = new Entity(messageBus);
    camera->Add(new Transform());
    camera->Add(new Camera(JamJar::Color(0, 0, 0)));
}
