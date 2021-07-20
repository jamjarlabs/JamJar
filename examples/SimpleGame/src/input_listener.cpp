#include "input_listener.hpp"
#include "message/message_payload.hpp"
#include "standard/sdl2_input/sdl2_input_system.hpp"
#include <SDL2/SDL.h>
#include <iostream>

InputListener::InputListener(JamJar::MessageBus *messageBus) : JamJar::System(messageBus) {
    this->messageBus->Subscribe(this, JamJar::Standard::SDL2InputSystem::MESSAGE_KEYBOARD_EVENT);
    this->messageBus->Subscribe(this, JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_CLICK_EVENT);
    this->messageBus->Subscribe(this, JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_MOTION_EVENT);
    this->messageBus->Subscribe(this, JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_WHEEL_EVENT);
}

void InputListener::OnMessage(JamJar::Message *message) {
    System::OnMessage(message);
    switch (message->type) {
    case JamJar::Standard::SDL2InputSystem::MESSAGE_KEYBOARD_EVENT: {
        auto *eventMessage = static_cast<JamJar::MessagePayload<std::unique_ptr<SDL_Event>> *>(message);
        auto event = eventMessage->payload.get();
        if (event->key.type == SDL_KEYUP) {
            std::cout << "listener got key up: " << SDL_GetKeyName(event->key.keysym.sym) << std::endl;
        } else {
            std::cout << "listener got key down: " << SDL_GetKeyName(event->key.keysym.sym) << std::endl;
        }
        break;
    }
    case JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_CLICK_EVENT: {
        auto *eventMessage = static_cast<JamJar::MessagePayload<std::unique_ptr<SDL_Event>> *>(message);
        auto event = eventMessage->payload.get();
        auto action = "down";
        if (event->type == SDL_MOUSEBUTTONUP) {
            action = "up";
        }
        if (event->button.button == SDL_BUTTON_LEFT) {
            std::cout << "listener got left mouse button " << action << std::endl;
        } else if (event->button.button == SDL_BUTTON_RIGHT) {
            std::cout << "listener got right mouse button " << action << std::endl;
        } else {
            std::cout << "listener got a different mouse button " << action << std::endl;
        }
        break;
    }
    case JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_MOTION_EVENT: {
        auto *eventMessage = static_cast<JamJar::MessagePayload<std::unique_ptr<SDL_Event>> *>(message);
        auto event = eventMessage->payload.get();
        // std::cout << "mouse move, x: " << event->motion.x << ", y:" << event->motion.y << std::endl;
        break;
    }
    case JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_WHEEL_EVENT: {
        auto *eventMessage = static_cast<JamJar::MessagePayload<std::unique_ptr<SDL_Event>> *>(message);
        auto event = eventMessage->payload.get();
        if (event->wheel.y > 0) {
            std::cout << "mouse wheel up" << std::endl;
        } else if (event->wheel.y < 0) {
            std::cout << "mouse wheel down" << std::endl;
        }
        break;
    }
    }
}
