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

MouseInput::MouseInput(JamJar::MessageBus *messageBus) : Game(messageBus) {}

void MouseInput::OnStart() {

    this->messageBus->Publish(
        std::make_unique<JamJar::MessagePayload<std::unique_ptr<JamJar::Standard::FileTextureRequest>>>(
            JamJar::Standard::FileTextureSystem::MESSAGE_REQUEST_FILE_TEXTURE_LOAD,
            std::unique_ptr<JamJar::Standard::FileTextureRequest>(new JamJar::Standard::FileTextureRequest(
                {.key = JamJar::hash("smiley"),
                 .path = "/assets/texture.png",
                 .properties = JamJar::TextureProperties(
                     {.minFilter = JamJar::TextureFilter::NEAREST, .magFilter = JamJar::TextureFilter::NEAREST})}))));

    auto camera = new JamJar::Entity(messageBus);
    camera->Add(std::move(std::make_unique<JamJar::Standard::_2D::Transform>()));
    camera->Add(std::move(std::make_unique<JamJar::Standard::_2D::Camera>(JamJar::Color(0, 0, 0))));
}
