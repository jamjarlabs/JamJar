#ifndef STANDARD_RENDER_RENDER_SYSTEM_HPP
#define STANDARD_RENDER_RENDER_SYSTEM_HPP

#include "hash.hpp"
#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "standard/render/renderable.hpp"
#include "system/map_system.hpp"

namespace JamJar {
namespace Standard {
class RenderSystem : public MapSystem {
  public:
    constexpr static uint32_t MESSAGE_LOAD_RENDERABLES = hash("jamjar_load_renderables");
    explicit RenderSystem(MessageBus *messageBus, Evaluator evaluator);
    void OnMessage(Message *message) override;

  protected:
    virtual void render(float alpha) = 0;

    std::vector<Renderable> renderables{};
};
}; // namespace Standard
}; // namespace JamJar

#endif
