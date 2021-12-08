#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include "emscripten/html5.h"
#include <stdexcept>
#include <string>
#endif

namespace JamJar {
SDL_Window *GetWindow(const char *title);
SDL_Window *GetWindow(const char *title, int width, int height);
SDL_Window *GetWindow(const char *title, int width, int height, int x, int y);
SDL_Window *GetWindow(const char *title, int width, int height, int x, int y, Uint32 flags);
void DestroyWindow(SDL_Window *window);
#ifdef __EMSCRIPTEN__

class CanvasInitializationException : public std::runtime_error {
  public:
    explicit CanvasInitializationException(std::string const &msg) : std::runtime_error(msg) {}
};

EMSCRIPTEN_WEBGL_CONTEXT_HANDLE GetCanvasContext();
EMSCRIPTEN_WEBGL_CONTEXT_HANDLE GetCanvasContext(const char *target);
EMSCRIPTEN_WEBGL_CONTEXT_HANDLE GetCanvasContext(const char *target, EmscriptenWebGLContextAttributes *attributes);
#endif
}; // namespace JamJar

#endif
