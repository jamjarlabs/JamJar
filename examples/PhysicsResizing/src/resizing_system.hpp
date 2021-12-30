#ifndef RESIZING_SYSTEM_HPP
#define RESIZING_SYSTEM_HPP

#include "component/component.hpp"
#include "entity/entity.hpp"
#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "system/vector_system.hpp"
#include <vector>

class ResizingSystem : public JamJar::VectorSystem {
  public:
    explicit ResizingSystem(JamJar::MessageBus *messageBus);

  protected:
    void update(float deltaTime) override;

  private:
    static bool evaluator(JamJar::Entity *entity, const std::vector<JamJar::Component *> &components);
};

#endif
