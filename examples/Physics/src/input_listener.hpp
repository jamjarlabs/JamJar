#ifndef INPUT_LISTENER_HPP
#define INPUT_LISTENER_HPP

#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "system/vector_system.hpp"
#include <SDL2/SDL.h>

class InputListener : public JamJar::VectorSystem {
  public:
    explicit InputListener(JamJar::MessageBus *messageBus, SDL_Window *window);
    void OnMessage(JamJar::Message *message) override;

  private:
    SDL_Window *window;
    static bool evaluator(JamJar::Entity *entity, const std::vector<JamJar::Component *> &components);
};

#endif
