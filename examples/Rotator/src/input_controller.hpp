#ifndef INPUT_CONTROLLER_HPP
#define INPUT_CONTROLLER_HPP

#include "geometry/vector_2d.hpp"
#include "hash.hpp"
#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "system/bucket_system.hpp"
#include <SDL2/SDL.h>

class InputController : public JamJar::BucketSystem {
  public:
    explicit InputController(JamJar::MessageBus *messageBus, SDL_Window *window);
    void OnMessage(JamJar::Message *message) override;

  private:
    constexpr static uint32_t CAMERA_BUCKET = JamJar::hash("camera");
    constexpr static uint32_t PLAYER_BUCKET = JamJar::hash("player");
    SDL_Window *window;
    static std::optional<uint32_t> evaluator(JamJar::Entity *entity,
                                             const std::vector<JamJar::Component *> &components);

    static float getOrientationBetweenPoints(JamJar::Vector2D start, JamJar::Vector2D end);
};

#endif
