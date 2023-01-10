#ifndef POINTER_CONTROLLER_HPP
#define POINTER_CONTROLLER_HPP

#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "system/bucket_system.hpp"
#include <SDL2/SDL.h>

class PointerController : public JamJar::BucketSystem {
  public:
    explicit PointerController(JamJar::MessageBus *messageBus, SDL_Window *window);
    void OnMessage(JamJar::Message *message) override;

  private:
    constexpr static uint32_t CAMERA_BUCKET = JamJar::hash("camera");
    constexpr static uint32_t CURSOR_BUCKET = JamJar::hash("cursor");
    SDL_Window *window;
    static std::optional<uint32_t> evaluator(JamJar::Entity *entity,
                                             const std::vector<JamJar::Component *> &components);
};

#endif
