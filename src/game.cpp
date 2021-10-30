#include "game.hpp"
#include "hash.hpp"
#include "message/message.hpp"
#include "message/message_payload.hpp"
#include "system/system.hpp"
#include "box2d/box2d.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <stdio.h>
#endif

const float MICROSECOND_TO_SECOND_CONVERSION = 1000000;

constexpr std::chrono::microseconds FRAMETIME_CAP = std::chrono::microseconds(250000);

JamJar::Game::Game(JamJar::MessageBus *messageBus)
    : messageBus(messageBus), isRunning(false), m_accumulator(std::chrono::microseconds(0)),
      m_currentTime(std::chrono::high_resolution_clock::now()) {
    messageBus->Subscribe(this, JamJar::Game::MESSAGE_STOP_GAME);
}

void JamJar::Game::Start() {
    this->OnStart();
    this->isRunning = true;
    this->startLoop();
}

void JamJar::Game::stop() {
    this->OnStop();
    this->isRunning = false;
}

void JamJar::Game::OnMessage(JamJar::Message *message) {
    switch (message->type) {
    case JamJar::Game::MESSAGE_STOP_GAME: {
        this->stop();
        return;
    }
    }
}

bool JamJar::Game::Loop(std::chrono::high_resolution_clock::time_point timestamp) {
    if (!this->isRunning) {
        return false;
    }

    // Calculate time since last frame.
    auto timeDifference = timestamp - this->m_currentTime;
    auto frameTime = std::chrono::duration_cast<std::chrono::microseconds>(timeDifference);
    if (frameTime > FRAMETIME_CAP) {
        // If frametime gets execssive, cap it.
        frameTime = FRAMETIME_CAP;
    }

    auto timeStep = std::chrono::microseconds(TIME_STEP);

    this->m_currentTime = timestamp;

    this->m_accumulator += frameTime;

    while (this->m_accumulator >= timeStep) {
        this->messageBus->Publish(std::make_unique<JamJar::MessagePayload<float>>(
            JamJar::System::MESSAGE_UPDATE, float(TIME_STEP) / MICROSECOND_TO_SECOND_CONVERSION));
        this->messageBus->Dispatch();
        this->m_accumulator -= timeStep;
    }

    // Alpha constant for interpolation calculations
    auto alpha = float(this->m_accumulator.count()) / float(TIME_STEP);

    // Pre-render and dispatch, must be immediately dispatched to allow pre-render systems to
    // send messages to the renderer before the actual render call.
    this->messageBus->Publish(std::make_unique<JamJar::MessagePayload<float>>(JamJar::Game::MESSAGE_PRE_RENDER, alpha));
    this->messageBus->Dispatch();

    // Render
    this->messageBus->Publish(std::make_unique<JamJar::MessagePayload<float>>(JamJar::Game::MESSAGE_RENDER, alpha));

    // Post render
    this->messageBus->Publish(
        std::make_unique<JamJar::MessagePayload<float>>(JamJar::Game::MESSAGE_POST_RENDER, alpha));
    this->messageBus->Dispatch();
    return true;
}

void JamJar::Game::OnStart() {}
void JamJar::Game::OnStop() {}

#ifdef __EMSCRIPTEN__

EM_BOOL loopWrapper(double timestamp, void *userData) {
    auto game = static_cast<JamJar::Game *>(userData);
    auto now = std::chrono::high_resolution_clock::now();
    if (game->Loop(now)) {
        emscripten_request_animation_frame(loopWrapper, game);
    }
    return EM_TRUE;
}

void JamJar::Game::startLoop() {
    this->m_currentTime = std::chrono::high_resolution_clock::now();
    loopWrapper(0, this);
}

#else
void JamJar::Game::startLoop() {
    this->m_currentTime = std::chrono::high_resolution_clock::now();
    bool running = true;
    while (running) {
        auto now = std::chrono::high_resolution_clock::now();
        running = this->Loop(now);
    }
}
#endif
