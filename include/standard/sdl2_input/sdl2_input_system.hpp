#ifndef STANDARD_SDL2_INPUT_SDL2_INPUT_SYSTEM_HPP
#define STANDARD_SDL2_INPUT_SDL2_INPUT_SYSTEM_HPP

#include "hash.hpp"
#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "system/system.hpp"

namespace JamJar::Standard {
class SDL2InputSystem : public System {
  public:
    constexpr static uint32_t MESSAGE_KEYBOARD_EVENT = hash("jamjar_sdl2_keyboard_event");
    constexpr static uint32_t MESSAGE_MOUSE_CLICK_EVENT = hash("jamjar_sdl2_mouse_click_event");
    constexpr static uint32_t MESSAGE_MOUSE_MOTION_EVENT = hash("jamjar_sdl2_mouse_motion_event");
    constexpr static uint32_t MESSAGE_MOUSE_WHEEL_EVENT = hash("jamjar_sdl2_mouse_wheel_event");
    explicit SDL2InputSystem(MessageBus *messageBus);

  protected:
    void update(float deltaTime) override;
};
}; // namespace JamJar::Standard

#endif
