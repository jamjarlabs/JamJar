#include "standard/sdl2_input/sdl2_input_system.hpp"
#include "message/message_payload.hpp"
#include <iostream>
#include <unordered_map>

const std::unordered_map<Uint32, uint32_t> MOUSE_MESSAGES = {
    {SDL_MOUSEBUTTONDOWN, JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_BUTTON_DOWN_EVENT},
    {SDL_MOUSEBUTTONUP, JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_BUTTON_UP_EVENT},
    {SDL_MOUSEMOTION, JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_MOTION_EVENT},
    {SDL_MOUSEWHEEL, JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_WHEEL_EVENT}};

const std::unordered_map<Uint32, JamJar::Standard::SDL2MouseEventType> MOUSE_TYPES = {
    {SDL_MOUSEBUTTONDOWN, JamJar::Standard::SDL2MouseEventType::MOUSE_BUTTON_DOWN},
    {SDL_MOUSEBUTTONUP, JamJar::Standard::SDL2MouseEventType::MOUSE_BUTTON_UP},
    {SDL_MOUSEMOTION, JamJar::Standard::SDL2MouseEventType::MOUSE_MOTION},
    {SDL_MOUSEWHEEL, JamJar::Standard::SDL2MouseEventType::MOUSE_WHEEL}};

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
            auto msg = std::make_unique<JamJar::MessagePayload<SDL_Event>>(
                JamJar::Standard::SDL2InputSystem::MESSAGE_KEYBOARD_EVENT, event);
            this->messageBus->Publish(std::move(msg));
            break;
        }
        case SDL_MOUSEWHEEL:
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            int x, y;
            SDL_GetMouseState(&x, &y);
            auto msg = std::make_unique<JamJar::MessagePayload<SDL2MouseEvent>>(
                MOUSE_MESSAGES.at(event.type),
                SDL2MouseEvent(MOUSE_TYPES.at(event.type), Vector2D(static_cast<float>(x), static_cast<float>(y)),
                               event));
            this->messageBus->Publish(std::move(msg));
            break;
        }
        }
    }
}

JamJar::Standard::SDL2MouseEvent::SDL2MouseEvent(JamJar::Standard::SDL2MouseEventType type, JamJar::Vector2D position,
                                                 SDL_Event event)
    : type(type), position(position), event(event) {}
