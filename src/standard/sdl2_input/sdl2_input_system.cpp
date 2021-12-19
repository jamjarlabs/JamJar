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

const std::unordered_map<Uint8, JamJar::Standard::SDL2MouseButton> MOUSE_BUTTONS = {
    {SDL_BUTTON_LEFT, JamJar::Standard::SDL2MouseButton::LEFT},
    {SDL_BUTTON_RIGHT, JamJar::Standard::SDL2MouseButton::RIGHT},
    {SDL_BUTTON_MIDDLE, JamJar::Standard::SDL2MouseButton::MIDDLE},
    {SDL_BUTTON_X1, JamJar::Standard::SDL2MouseButton::X1},
    {SDL_BUTTON_X2, JamJar::Standard::SDL2MouseButton::X2}};

JamJar::Standard::SDL2InputSystem::SDL2InputSystem(MessageBus *messageBus) : System(messageBus) {}

void JamJar::Standard::SDL2InputSystem::update(float deltaTime) {
    bool polling = true;
    while (polling) {
        SDL_Event event;
        if (SDL_PollEvent(&event) == 0) {
            polling = false;
            break;
        }
        switch (event.type) {
        case SDL_KEYDOWN: {
            auto msg = std::make_unique<JamJar::MessagePayload<SDL2KeyEvent>>(
                JamJar::Standard::SDL2InputSystem::MESSAGE_KEY_DOWN_EVENT,
                SDL2KeyEvent(SDL2KeyEventType::KEY_DOWN, event, SDL_GetKeyName(event.key.keysym.sym),
                             event.key.repeat == 1));
            this->messageBus->Publish(std::move(msg));
            break;
        }
        case SDL_KEYUP: {
            auto msg = std::make_unique<JamJar::MessagePayload<SDL2KeyEvent>>(
                JamJar::Standard::SDL2InputSystem::MESSAGE_KEY_UP_EVENT,
                SDL2KeyEvent(SDL2KeyEventType::KEY_UP, event, SDL_GetKeyName(event.key.keysym.sym),
                             event.key.repeat == 1));
            this->messageBus->Publish(std::move(msg));
            break;
        }
        case SDL_MOUSEWHEEL: {
            int x, y;
            SDL_GetMouseState(&x, &y);
            auto msg = std::make_unique<JamJar::MessagePayload<SDL2MouseEvent>>(
                MOUSE_MESSAGES.at(event.type),
                SDL2MouseEvent(MOUSE_TYPES.at(event.type), Vector2D(static_cast<float>(x), static_cast<float>(y)),
                               event));
            this->messageBus->Publish(std::move(msg));
        }
        case SDL_MOUSEMOTION: {
            auto msg = std::make_unique<JamJar::MessagePayload<SDL2MouseEvent>>(
                MOUSE_MESSAGES.at(event.type),
                SDL2MouseEvent(MOUSE_TYPES.at(event.type),
                               Vector2D(static_cast<float>(event.motion.x), static_cast<float>(event.motion.y)),
                               event));
            this->messageBus->Publish(std::move(msg));
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            int x, y;
            SDL_GetMouseState(&x, &y);
            auto msg = std::make_unique<JamJar::MessagePayload<SDL2MouseEvent>>(
                MOUSE_MESSAGES.at(event.type),
                SDL2MouseEvent(MOUSE_TYPES.at(event.type), MOUSE_BUTTONS.at(event.button.button),
                               Vector2D(static_cast<float>(x), static_cast<float>(y)), event));
            this->messageBus->Publish(std::move(msg));
            break;
        }
        }
    }
}

JamJar::Standard::SDL2MouseEvent::SDL2MouseEvent(JamJar::Standard::SDL2MouseEventType type,
                                                 JamJar::Standard::SDL2MouseButton button, JamJar::Vector2D position,
                                                 SDL_Event event)
    : type(type), button(button), position(position), event(event) {}

JamJar::Standard::SDL2MouseEvent::SDL2MouseEvent(JamJar::Standard::SDL2MouseEventType type, JamJar::Vector2D position,
                                                 SDL_Event event)
    : type(type), button(std::nullopt), position(position), event(event) {}

JamJar::Standard::SDL2KeyEvent::SDL2KeyEvent(JamJar::Standard::SDL2KeyEventType type, SDL_Event event, const char *key,
                                             bool repeat)
    : type(type), event(event), key(key), repeat(repeat) {}
