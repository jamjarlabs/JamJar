#include "input_controller.hpp"
#include "message/message_payload.hpp"
#include "player.hpp"
#include "render/material.hpp"
#include "standard/2d/camera/camera.hpp"
#include "standard/2d/sprite/sprite.hpp"
#include "standard/2d/transform/transform.hpp"
#include "standard/sdl2_input/sdl2_input_system.hpp"
#include <SDL2/SDL.h>

const float PI = 3.14159265358979323846;

std::optional<uint32_t> InputController::evaluator(JamJar::Entity *entity, const std::vector<JamJar::Component *> &components) {
    for (const auto &component : components) {
        if (component->key == JamJar::Standard::_2D::Camera::KEY) {
            return std::optional<uint32_t>(InputController::CAMERA_BUCKET);
        }
        if (component->key == Player::KEY) {
            return std::optional<uint32_t>(InputController::PLAYER_BUCKET);
        }
    }
    return std::optional<uint32_t>(std::nullopt);
}

InputController::InputController(JamJar::MessageBus *messageBus, SDL_Window *window)
    : JamJar::BucketSystem(messageBus, InputController::evaluator), window(window) {
    this->messageBus->Subscribe(this, JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_MOTION_EVENT);
}

void InputController::OnMessage(JamJar::Message *message) {
    BucketSystem::OnMessage(message);
    switch (message->type) {
    case JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_MOTION_EVENT: {
        auto *eventMessage = static_cast<JamJar::MessagePayload<JamJar::Standard::SDL2MouseEvent> *>(message);
        auto event = eventMessage->payload;

        if (this->entities.count(CAMERA_BUCKET) <= 0 || this->entities.count(PLAYER_BUCKET) <= 0) {
            break;
        }

        auto cameras = this->entities.at(CAMERA_BUCKET);
        auto cameraEntity = cameras.front();

        auto transform = cameraEntity.Get<JamJar::Standard::_2D::Transform>();
        auto camera = cameraEntity.Get<JamJar::Standard::_2D::Camera>();

        auto worldPos = JamJar::Standard::_2D::MousePositionToWorldPosition(event.position, transform->position, camera,
                                                                             this->window);

        auto players = this->entities.at(PLAYER_BUCKET);

        for (auto& player : players) {
            auto transform = player.Get<JamJar::Standard::_2D::Transform>();
            transform->angle = InputController::getOrientationBetweenPoints(transform->position, worldPos);
        }
        break;
    }
    }
}

float InputController::getOrientationBetweenPoints(JamJar::Vector2D start, JamJar::Vector2D end) {
    float theta = atan2(end.x - start.x, start.y - end.y);

    if (theta < 0) {
        theta += PI * 2;
    }

    return theta + PI;
}
