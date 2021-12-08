#include "input_listener.hpp"
#include "message/message_payload.hpp"
#include "standard/sdl2_input/sdl2_input_system.hpp"
#include "standard/window/window_system.hpp"
#include <SDL2/SDL.h>
#include <iostream>

InputListener::InputListener(JamJar::MessageBus *messageBus) : JamJar::System(messageBus) {
    this->messageBus->Subscribe(this, JamJar::Standard::SDL2InputSystem::MESSAGE_KEYBOARD_EVENT);
}

void InputListener::OnMessage(JamJar::Message *message) {
    System::OnMessage(message);
    switch (message->type) {
    case JamJar::Standard::SDL2InputSystem::MESSAGE_KEYBOARD_EVENT: {
        auto *eventMessage = static_cast<JamJar::MessagePayload<SDL_Event> *>(message);
        auto event = eventMessage->payload;
        if (event.key.type == SDL_KEYDOWN) {
            auto key = SDL_GetKeyName(event.key.keysym.sym);
            if (std::strcmp(key, "F") == 0) {
                auto msg =
                    std::make_unique<JamJar::Message>(JamJar::Standard::WindowSystem::MESSAGE_REQUEST_ENTER_FULLSCREEN);
                this->messageBus->Publish(std::move(msg));
                break;
            }
            if (std::strcmp(key, "E") == 0) {
                auto msg =
                    std::make_unique<JamJar::Message>(JamJar::Standard::WindowSystem::MESSAGE_REQUEST_EXIT_FULLSCREEN);
                this->messageBus->Publish(std::move(msg));
                break;
            }
        }
        break;
    }
    }
}
