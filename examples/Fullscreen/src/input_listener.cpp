#include "input_listener.hpp"
#include "message/message_payload.hpp"
#include "standard/sdl2_input/sdl2_input_system.hpp"
#include "standard/window/window_system.hpp"
#include <iostream>

InputListener::InputListener(JamJar::MessageBus *messageBus) : JamJar::System(messageBus) {
    this->messageBus->Subscribe(this, JamJar::Standard::SDL2InputSystem::MESSAGE_KEY_UP_EVENT);
}

void InputListener::OnMessage(JamJar::Message *message) {
    System::OnMessage(message);
    switch (message->type) {
    case JamJar::Standard::SDL2InputSystem::MESSAGE_KEY_UP_EVENT: {
        auto *eventMessage = static_cast<JamJar::MessagePayload<JamJar::Standard::SDL2KeyEvent> *>(message);
        auto event = eventMessage->payload;
        if (event.key == "F") {
            auto msg =
                std::make_unique<JamJar::Message>(JamJar::Standard::WindowSystem::MESSAGE_REQUEST_ENTER_FULLSCREEN);
            this->messageBus->Publish(std::move(msg));
            break;
        }
        if (event.key == "E") {
            auto msg =
                std::make_unique<JamJar::Message>(JamJar::Standard::WindowSystem::MESSAGE_REQUEST_EXIT_FULLSCREEN);
            this->messageBus->Publish(std::move(msg));
            break;
        }
        break;
    }
    }
}
