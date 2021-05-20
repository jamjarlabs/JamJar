#ifndef GAME_HPP
#define GAME_HPP

#include "hash.hpp"
#include "message/listener.hpp"
#include "message/message.hpp"
#include "message/message_bus.hpp"
#include <chrono>

namespace JamJar {
class Game : public Listener {

  private:
    constexpr static int64_t TIME_STEP = 16700;

    std::chrono::high_resolution_clock::time_point m_currentTime{};
    std::chrono::microseconds m_accumulator{};

    void startLoop();
    void stop();

  protected:
    bool m_isRunning;
    MessageBus *m_messageBus;
    virtual void OnStart();
    virtual void OnStop();

  public:
    constexpr static uint32_t MESSAGE_STOP_GAME = hash("jamjar_stop_game");
    constexpr static uint32_t MESSAGE_PRE_RENDER = hash("jamjar_pre_render");
    constexpr static uint32_t MESSAGE_RENDER = hash("jamjar_render");
    constexpr static uint32_t MESSAGE_POST_RENDER = hash("jamjar_post_render");
    explicit Game(MessageBus *messageBus);
    void Start();
    void OnMessage(Message *message) override;
    bool Loop(std::chrono::high_resolution_clock::time_point timestamp);
};
}; // namespace JamJar

#endif
