#include "mouse_input.hpp"
#include "entity/entity.hpp"
#include "game.hpp"
#include "geometry/vector_2d.hpp"
#include "message/message_payload.hpp"
#include "standard/2d/camera/camera.hpp"
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
using JamJar::Standard::_2D::Transform;

MouseInput::MouseInput(JamJar::MessageBus *messageBus) : Game(messageBus) {}

void MouseInput::OnStart() {
    LoadTexture(this->messageBus,
                new FileTextureRequest({.key = JamJar::hash("smiley"),
                                        .path = "/assets/texture.png",
                                        .properties = TextureProperties({.minFilter = TextureFilter::NEAREST,
                                                                         .magFilter = TextureFilter::NEAREST})}));

    auto camera = new Entity(messageBus);
    camera->Add(new Transform());
    camera->Add(new Camera(Color(0, 0, 0)));
}
