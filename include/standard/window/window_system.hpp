#ifndef STANDARD_WINDOW_WINDOW_SYSTEM_HPP
#define STANDARD_WINDOW_WINDOW_SYSTEM_HPP
#ifdef __EMSCRIPTEN__

#include "emscripten/html5.h"
#include "hash.hpp"
#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "system/system.hpp"
#include <SDL2/SDL.h>

namespace JamJar::Standard {

struct WindowSystemProperties {
    double aspectRatio = (double)16 / (double)9;
    int maxResolutionX = -1;
    int maxResolutionY = -1;
    bool showCursor = true;
};

class WindowSystem : public System {
  public:
    constexpr static uint32_t MESSAGE_SET_ASPECT_RATIO = hash("jamjar_window_set_aspect_ratio");
    constexpr static uint32_t MESSAGE_SET_MAX_RESOLUTION = hash("jamjar_window_set_max_resolution");
    constexpr static uint32_t MESSAGE_REQUEST_ENTER_FULLSCREEN = hash("jamjar_window_request_enter_fullscreen");
    constexpr static uint32_t MESSAGE_REQUEST_EXIT_FULLSCREEN = hash("jamjar_window_request_exit_fullscreen");
    constexpr static uint32_t MESSAGE_ENTER_FULLSCREEN = hash("jamjar_window_enter_fullscreen");
    constexpr static uint32_t MESSAGE_EXIT_FULLSCREEN = hash("jamjar_window_exit_fullscreen");
    constexpr static uint32_t MESSAGE_ENTER_FULLSCREEN_FAILURE = hash("jamjar_window_enter_fullscreen_failure");
    constexpr static uint32_t MESSAGE_EXIT_FULLSCREEN_FAILURE = hash("jamjar_window_exit_fullscreen_failure");
    constexpr static uint32_t MESSAGE_REQUEST_HIDE_CURSOR = hash("jamjar_window_request_hide_cursor");
    constexpr static uint32_t MESSAGE_REQUEST_SHOW_CURSOR = hash("jamjar_window_request_show_cursor");
    constexpr static uint32_t MESSAGE_HIDE_CURSOR = hash("jamjar_window_hide_cursor");
    constexpr static uint32_t MESSAGE_SHOW_CURSOR = hash("jamjar_window_show_cursor");

    explicit WindowSystem(MessageBus *messageBus, SDL_Window *window, const char *wrapperID);
    explicit WindowSystem(MessageBus *messageBus, SDL_Window *window, const char *wrapperID,
                          WindowSystemProperties properties);
    void OnMessage(Message *message) override;
    void ResizeEvent();
    void FullscreenEvent(bool isFullscreen);

  private:
    void resizeCanvas();
    SDL_Window *window;
    const char *wrapperID;
    int maxResolutionX;
    int maxResolutionY;
    double aspectRatio;
    bool isFullscreen;
    bool showCursor;
};
}; // namespace JamJar::Standard

#endif
#endif
