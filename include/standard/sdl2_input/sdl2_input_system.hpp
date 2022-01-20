#ifndef STANDARD_SDL2_INPUT_SDL2_INPUT_SYSTEM_HPP
#define STANDARD_SDL2_INPUT_SDL2_INPUT_SYSTEM_HPP

#include "geometry/vector_2d.hpp"
#include "hash.hpp"
#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "system/system.hpp"
#include <SDL2/SDL.h>
#include <optional>
#include <string>

namespace JamJar::Standard {
class SDL2InputSystem : public System {
  public:
    constexpr static uint32_t MESSAGE_KEY_UP_EVENT = hash("jamjar_sdl2_key_up_event");
    constexpr static uint32_t MESSAGE_KEY_DOWN_EVENT = hash("jamjar_sdl2_key_down_event");
    constexpr static uint32_t MESSAGE_MOUSE_BUTTON_DOWN_EVENT = hash("jamjar_sdl2_mouse_button_down_event");
    constexpr static uint32_t MESSAGE_MOUSE_BUTTON_UP_EVENT = hash("jamjar_sdl2_mouse_button_up_event");
    constexpr static uint32_t MESSAGE_MOUSE_MOTION_EVENT = hash("jamjar_sdl2_mouse_motion_event");
    constexpr static uint32_t MESSAGE_MOUSE_WHEEL_EVENT = hash("jamjar_sdl2_mouse_wheel_event");
    explicit SDL2InputSystem(MessageBus *messageBus);

  protected:
    void update(float deltaTime) override;
};

enum SDL2MouseEventType { MOUSE_BUTTON_DOWN = 0, MOUSE_BUTTON_UP, MOUSE_MOTION, MOUSE_WHEEL };
enum SDL2MouseButton { LEFT = 0, MIDDLE, RIGHT, X1, X2 };

class SDL2MouseEvent {
  public:
    explicit SDL2MouseEvent(SDL2MouseEventType type, SDL2MouseButton button, const JamJar::Vector2D &position,
                            SDL_Event event);
    explicit SDL2MouseEvent(SDL2MouseEventType type, const JamJar::Vector2D &position, SDL_Event event);
    virtual ~SDL2MouseEvent() = default;

    SDL2MouseEventType type;
    std::optional<SDL2MouseButton> button;
    JamJar::Vector2D position;
    SDL_Event event;
};

enum SDL2KeyEventType { KEY_DOWN = 0, KEY_UP };

class SDL2KeyEvent {
  public:
    explicit SDL2KeyEvent(SDL2KeyEventType type, SDL_Event event, std::string key, bool repeat);

    SDL2KeyEventType type;
    SDL_Event event;
    std::string key;
    bool repeat;
};

}; // namespace JamJar::Standard

#endif
