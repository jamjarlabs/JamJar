#ifndef STANDARD_2D_SPRITE_SPRITE_SYSTEM_HPP
#define STANDARD_2D_SPRITE_SPRITE_SYSTEM_HPP

#include "component/component.hpp"
#include "entity/entity.hpp"
#include "message/message_bus.hpp"
#include "system/evaluator.hpp"
#include "system/map_system.hpp"

namespace JamJar::Standard::_2D {
class SpriteSystem : public MapSystem {
  public:
    explicit SpriteSystem(MessageBus *messageBus);
    void OnMessage(Message *message) override;

  private:
    void preRender(float alpha);
    static bool evaluator(Entity *entity, const std::vector<JamJar::Component *> &components);
};
}; // namespace JamJar::Standard::_2D

#endif
