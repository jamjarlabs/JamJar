#ifndef INPUT_CONTROLLER_HPP
#define INPUT_CONTROLLER_HPP

#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "system/single_entity_system.hpp"
#include <SDL2/SDL.h>

class InputController : public JamJar::SingleEntitySystem {
  public:
    explicit InputController(JamJar::MessageBus *messageBus, SDL_Window *window);
    void OnMessage(JamJar::Message *message) override;

  private:
    SDL_Window *window;
    static bool evaluator(JamJar::Entity *entity, const std::vector<JamJar::Component *> &components);
};

#endif
