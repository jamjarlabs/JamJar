#include "pointer_controller.hpp"
#include "message/message_payload.hpp"
#include "render/material.hpp"
#include "standard/2d/camera/camera.hpp"
#include "standard/2d/sprite/sprite.hpp"
#include "standard/2d/transform/transform.hpp"
#include "standard/sdl2_input/sdl2_input_system.hpp"
#include "standard/window/window_system.hpp"
#include <SDL2/SDL.h>
#include <iostream>

std::optional<uint32_t> PointerController::evaluator(JamJar::Entity *entity,
                                                     const std::vector<JamJar::Component *> &components) {
    for (const auto &component : components) {
        if (component->key == JamJar::Standard::_2D::Camera::KEY) {
            return std::optional<uint32_t>(PointerController::CAMERA_BUCKET);
        }
        if (component->key == JamJar::Standard::_2D::Sprite::KEY) {
            return std::optional<uint32_t>(PointerController::CURSOR_BUCKET);
        }
    }
    return std::optional<uint32_t>(std::nullopt);
}

PointerController::PointerController(JamJar::MessageBus *messageBus, SDL_Window *window)
    : JamJar::BucketSystem(messageBus, PointerController::evaluator), window(window) {
    this->messageBus->Subscribe(this, JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_MOTION_EVENT);
    this->messageBus->Subscribe(this, JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_BUTTON_DOWN_EVENT);
}

void PointerController::OnMessage(JamJar::Message *message) {
    BucketSystem::OnMessage(message);
    switch (message->type) {
    case JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_MOTION_EVENT: {
        auto *eventMessage = static_cast<JamJar::MessagePayload<JamJar::Standard::SDL2MouseEvent> *>(message);
        auto event = eventMessage->payload;

        if (this->entities.count(CAMERA_BUCKET) <= 0 || this->entities.count(CURSOR_BUCKET) <= 0) {
            break;
        }

        auto cameras = this->entities.at(CAMERA_BUCKET);
        auto cameraEntity = cameras.front();

        auto cameraTransform = cameraEntity.Get<JamJar::Standard::_2D::Transform>();
        auto camera = cameraEntity.Get<JamJar::Standard::_2D::Camera>();

        auto worldPos = JamJar::Standard::_2D::MousePositionToWorldPosition(event.position, cameraTransform->position,
                                                                            camera, this->window);

        auto cursors = this->entities.at(CURSOR_BUCKET);
        auto cursor = cursors.front();

        auto cursorTransform = cursor.Get<JamJar::Standard::_2D::Transform>();

        cursorTransform->position.x = worldPos.x;
        cursorTransform->position.y = worldPos.y;
        break;
    }
    case JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_BUTTON_DOWN_EVENT: {
        auto *eventMessage = static_cast<JamJar::MessagePayload<JamJar::Standard::SDL2MouseEvent> *>(message);
        auto event = eventMessage->payload;

        if (event.button.value() == JamJar::Standard::SDL2MouseButton::LEFT) {
            this->messageBus->Publish(new JamJar::Message(JamJar::Standard::WindowSystem::MESSAGE_REQUEST_SHOW_CURSOR));
        }

        if (event.button.value() == JamJar::Standard::SDL2MouseButton::RIGHT) {
            this->messageBus->Publish(new JamJar::Message(JamJar::Standard::WindowSystem::MESSAGE_REQUEST_HIDE_CURSOR));
        }
    }
    }
}
