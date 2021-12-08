#include "window.hpp"

SDL_Window *JamJar::GetWindow(const char *title, int width, int height, int x, int y, Uint32 flags) {
    SDL_Init(SDL_INIT_VIDEO);
    return SDL_CreateWindow(title, x, y, width, height, flags);
}

SDL_Window *JamJar::GetWindow(const char *title, int width, int height, int x, int y) {
    return JamJar::GetWindow(title, width, height, x, y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
}

SDL_Window *JamJar::GetWindow(const char *title, int width, int height) {
    return JamJar::GetWindow(title, width, height, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED);
}

SDL_Window *JamJar::GetWindow(const char *title) { return JamJar::GetWindow(title, 1024, 576); }

void JamJar::DestroyWindow(SDL_Window *window) { SDL_DestroyWindow(window); }

#ifdef __EMSCRIPTEN__

EMSCRIPTEN_WEBGL_CONTEXT_HANDLE JamJar::GetCanvasContext(const char *target,
                                                         EmscriptenWebGLContextAttributes *attributes) {
    auto context = emscripten_webgl_create_context(target, attributes);
    auto res = emscripten_webgl_make_context_current(context);
    if (res != EMSCRIPTEN_RESULT_SUCCESS) {
        throw JamJar::CanvasInitializationException("Failed to make WebGL context the current context");
    }
    return context;
}

EMSCRIPTEN_WEBGL_CONTEXT_HANDLE JamJar::GetCanvasContext(const char *target) {
    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);
    return JamJar::GetCanvasContext(target, &attrs);
}

EMSCRIPTEN_WEBGL_CONTEXT_HANDLE JamJar::GetCanvasContext() { return JamJar::GetCanvasContext("#canvas"); }

#endif
