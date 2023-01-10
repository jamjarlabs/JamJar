#include "standard/window/window_system.hpp"
#ifdef __EMSCRIPTEN__
#include "message/message_payload.hpp"
#include <emscripten.h>
#include <iostream>
#include <utility>

const char *CANVAS_SELECTOR = "#canvas";

EM_BOOL resizeCallback(int eventType, const EmscriptenUiEvent *e, void *userData) {
    auto system = static_cast<JamJar::Standard::WindowSystem *>(userData);
    system->ResizeEvent();
    return 0;
}

EM_BOOL fullscreenCallback(int eventType, const EmscriptenFullscreenChangeEvent *e, void *userData) {
    auto system = static_cast<JamJar::Standard::WindowSystem *>(userData);
    system->FullscreenEvent(e->isFullscreen);
    return 0;
}

JamJar::Standard::WindowSystem::WindowSystem(MessageBus *messageBus, SDL_Window *window, const char *wrapperID,
                                             WindowSystemProperties properties)
    : System(messageBus), window(window), wrapperID(wrapperID), aspectRatio(properties.aspectRatio),
      isFullscreen(false), maxResolutionX(properties.maxResolutionX), maxResolutionY(properties.maxResolutionY),
      showCursor(properties.showCursor) {
    this->messageBus->Subscribe(this, WindowSystem::MESSAGE_SET_ASPECT_RATIO);
    this->messageBus->Subscribe(this, WindowSystem::MESSAGE_SET_MAX_RESOLUTION);
    this->messageBus->Subscribe(this, WindowSystem::MESSAGE_REQUEST_ENTER_FULLSCREEN);
    this->messageBus->Subscribe(this, WindowSystem::MESSAGE_REQUEST_EXIT_FULLSCREEN);
    this->messageBus->Subscribe(this, WindowSystem::MESSAGE_REQUEST_EXIT_FULLSCREEN);
    this->messageBus->Subscribe(this, WindowSystem::MESSAGE_REQUEST_HIDE_CURSOR);
    this->messageBus->Subscribe(this, WindowSystem::MESSAGE_REQUEST_SHOW_CURSOR);
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, true, resizeCallback);
    emscripten_set_fullscreenchange_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, this, true, fullscreenCallback);
    this->resizeCanvas();
}

JamJar::Standard::WindowSystem::WindowSystem(MessageBus *messageBus, SDL_Window *window, const char *wrapperID)
    : WindowSystem(messageBus, window, wrapperID, WindowSystemProperties()) {}

void JamJar::Standard::WindowSystem::FullscreenEvent(bool isFullscreen) {
    this->isFullscreen = isFullscreen;
    if (isFullscreen) {
        auto msg = std::make_unique<JamJar::Message>(JamJar::Standard::WindowSystem::MESSAGE_ENTER_FULLSCREEN);
        this->messageBus->Publish(std::move(msg));
    } else {
        auto msg = std::make_unique<JamJar::Message>(JamJar::Standard::WindowSystem::MESSAGE_EXIT_FULLSCREEN);
        this->messageBus->Publish(std::move(msg));
    }
    this->resizeCanvas();
}

void JamJar::Standard::WindowSystem::ResizeEvent() { this->resizeCanvas(); }

void JamJar::Standard::WindowSystem::OnMessage(JamJar::Message *message) {
    System::OnMessage(message);
    switch (message->type) {
    case WindowSystem::MESSAGE_SET_ASPECT_RATIO: {
        auto *setAspectRatioMessage = static_cast<JamJar::MessagePayload<double> *>(message);
        this->aspectRatio = setAspectRatioMessage->payload;
        this->resizeCanvas();
        break;
    }
    case WindowSystem::MESSAGE_SET_MAX_RESOLUTION: {
        auto *setMaxResolutionMessage = static_cast<JamJar::MessagePayload<std::pair<int, int>> *>(message);
        this->maxResolutionX = setMaxResolutionMessage->payload.first;
        this->maxResolutionY = setMaxResolutionMessage->payload.second;
        this->resizeCanvas();
        break;
    }
    case WindowSystem::MESSAGE_REQUEST_ENTER_FULLSCREEN: {
        int result = emscripten_request_fullscreen(CANVAS_SELECTOR, true);
        if (result != EMSCRIPTEN_RESULT_SUCCESS && result != EMSCRIPTEN_RESULT_DEFERRED) {
            auto msg =
                std::make_unique<JamJar::Message>(JamJar::Standard::WindowSystem::MESSAGE_ENTER_FULLSCREEN_FAILURE);
            this->messageBus->Publish(std::move(msg));
        }
        break;
    }
    case WindowSystem::MESSAGE_REQUEST_EXIT_FULLSCREEN: {
        int result = emscripten_exit_fullscreen();
        if (result != EMSCRIPTEN_RESULT_SUCCESS && result != EMSCRIPTEN_RESULT_DEFERRED) {
            this->messageBus->Publish(
                new JamJar::Message(JamJar::Standard::WindowSystem::MESSAGE_EXIT_FULLSCREEN_FAILURE));
        }
        break;
    }
    case WindowSystem::MESSAGE_REQUEST_HIDE_CURSOR: {
        SDL_ShowCursor(SDL_DISABLE);
        this->showCursor = false;
        this->messageBus->Publish(new JamJar::Message(JamJar::Standard::WindowSystem::MESSAGE_SHOW_CURSOR));
        break;
    }
    case WindowSystem::MESSAGE_REQUEST_SHOW_CURSOR: {
        SDL_ShowCursor(SDL_ENABLE);
        this->showCursor = true;
        this->messageBus->Publish(new JamJar::Message(JamJar::Standard::WindowSystem::MESSAGE_SHOW_CURSOR));
        break;
    }
    }
}

void JamJar::Standard::WindowSystem::resizeCanvas() {
    int maxWidth;
    int maxHeight;

    if (this->isFullscreen) {
        maxWidth = EM_ASM_INT({ return screen.width; }, this->wrapperID);
        maxHeight = EM_ASM_INT({ return screen.height; }, this->wrapperID);
    } else {
        maxWidth = EM_ASM_INT({ return document.getElementById(UTF8ToString($0)).clientWidth; }, this->wrapperID);
        maxHeight = EM_ASM_INT({ return document.getElementById(UTF8ToString($0)).clientHeight; }, this->wrapperID);
    }

    if (this->maxResolutionX != -1 && this->maxResolutionX < (int)maxWidth) {
        maxWidth = this->maxResolutionX;
    }
    if (this->maxResolutionY != -1 && this->maxResolutionY < (int)maxHeight) {
        maxWidth = this->maxResolutionY;
    }

    int width = maxWidth;
    int height = (int)((double)maxHeight * this->aspectRatio);

    if (width > height) {
        width = (int)height;
        height = maxHeight;
    } else {
        height = (int)((double)maxWidth * (1 / this->aspectRatio));
    }

    if (this->showCursor) {
        SDL_ShowCursor(SDL_ENABLE);
    } else {
        SDL_ShowCursor(SDL_DISABLE);
    }

    if (this->isFullscreen) {
        SDL_SetWindowSize(window, width, height);
    } else {
        // We need to tell SDL that we are not in full screen mode to stop it calling emscripten's fullscreen functions
        SDL_SetWindowFullscreen(window, 0);
        SDL_SetWindowSize(window, width, height);
    }
}

#endif
