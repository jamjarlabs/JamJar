#ifndef DESTROY_SYSTEM_HPP
#define DESTROY_SYSTEM_HPP

#include "component/component.hpp"
#include "entity/entity.hpp"
#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "system/bucket_map_system.hpp"
#include <vector>

class DestroySystem : public JamJar::BucketMapSystem {
  public:
    explicit DestroySystem(JamJar::MessageBus *messageBus);
    void OnMessage(JamJar::Message *message) override;

  protected:
    void update(float deltaTime) override;

  private:
    constexpr static uint32_t BOX_BUCKET = JamJar::hash("box");
    constexpr static uint32_t DESTRUCTOR_BUCKET = JamJar::hash("destructor");
    static std::optional<uint32_t> evaluator(JamJar::Entity *entity, const std::vector<JamJar::Component *> &components);
    float lastCreateTime;
};

#endif
