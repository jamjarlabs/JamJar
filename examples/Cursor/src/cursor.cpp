#include "cursor.hpp"
#include "entity/entity.hpp"
#include "game.hpp"
#include "geometry/vector_2d.hpp"
#include "message/message_payload.hpp"
#include "standard/2d/camera/camera.hpp"
#include "standard/2d/sprite/sprite.hpp"
#include "standard/2d/transform/transform.hpp"
#include "standard/file_texture/file_texture_request.hpp"
#include "standard/file_texture/file_texture_system.hpp"
#include <memory>

using JamJar::Color;
using JamJar::Entity;
using JamJar::Material;
using JamJar::Texture;
using JamJar::TextureFilter;
using JamJar::TextureProperties;
using JamJar::Vector2D;
using JamJar::Standard::FileTextureRequest;
using JamJar::Standard::FileTextureSystem;
using JamJar::Standard::LoadTexture;
using JamJar::Standard::_2D::Camera;
using JamJar::Standard::_2D::Sprite;
using JamJar::Standard::_2D::Transform;

Cursor::Cursor(JamJar::MessageBus *messageBus) : Game(messageBus) {}

void Cursor::OnStart() {
    LoadTexture(this->messageBus,
                new FileTextureRequest({.key = JamJar::hash("smiley"),
                                        .path = "/assets/texture.png",
                                        .properties = TextureProperties({.minFilter = TextureFilter::NEAREST,
                                                                         .magFilter = TextureFilter::NEAREST})}));

    auto camera = new Entity(messageBus);
    camera->Add(new Transform());
    camera->Add(new Camera(Color(1, 1, 1)));

    auto smiley = new JamJar::Entity(messageBus);
    smiley->Add(new Transform(Vector2D(0, 0), Vector2D(5, 5)));
    smiley->Add(new Sprite(Material(Color(0, 1, 1, 1), Texture(JamJar::hash("smiley")))));
}
