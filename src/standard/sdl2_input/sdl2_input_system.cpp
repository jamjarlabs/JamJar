#include "standard/sdl2_input/sdl2_input_system.hpp"
#include "message/message_payload.hpp"
#include <SDL2/SDL.h>
#include <iostream>

JamJar::Standard::SDL2InputSystem::SDL2InputSystem(MessageBus *messageBus) : System(messageBus) {}

void JamJar::Standard::SDL2InputSystem::update(float deltaTime) {
    bool polling = true;
    while (polling) {
        SDL_Event event;
        if (SDL_PollEvent(&event) == 0) {
            polling = false;
        }
        switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP: {
            auto msg = std::make_unique<JamJar::MessagePayload<std::unique_ptr<SDL_Event>>>(
                JamJar::Standard::SDL2InputSystem::MESSAGE_KEYBOARD_EVENT, std::make_unique<SDL_Event>(event));
            this->messageBus->Publish(std::move(msg));
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            auto msg = std::make_unique<JamJar::MessagePayload<std::unique_ptr<SDL_Event>>>(
                JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_CLICK_EVENT, std::make_unique<SDL_Event>(event));
            this->messageBus->Publish(std::move(msg));
            break;
        }
        case SDL_MOUSEWHEEL: {
            auto msg = std::make_unique<JamJar::MessagePayload<std::unique_ptr<SDL_Event>>>(
                JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_WHEEL_EVENT, std::make_unique<SDL_Event>(event));
            this->messageBus->Publish(std::move(msg));
            break;
        }
        case SDL_MOUSEMOTION: {
            auto msg = std::make_unique<JamJar::MessagePayload<std::unique_ptr<SDL_Event>>>(
                JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_MOTION_EVENT, std::make_unique<SDL_Event>(event));
            this->messageBus->Publish(std::move(msg));
            break;
        }
        }
    }
}
