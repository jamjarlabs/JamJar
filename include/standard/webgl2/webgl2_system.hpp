#ifdef __EMSCRIPTEN__
#ifndef STANDARD_WEBGL2_WEBGL2_SYSTEM_HPP
#define STANDARD_WEBGL2_WEBGL2_SYSTEM_HPP

#include "component/component.hpp"
#include "emscripten/html5.h"
#include "entity/entity.hpp"
#include "hash.hpp"
#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "message/listener.hpp"
#include "standard/render/render_system.hpp"
#include "system/evaluator.hpp"

namespace JamJar {
namespace Standard {
class WebGL2System : public RenderSystem {
  private:
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE m_context{};
    static bool evaluator(Entity *entity, std::vector<JamJar::Component *> components);

  protected:
    void render(float deltaTime) override;

  public:
    explicit WebGL2System(MessageBus *messageBus, EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context);
};
}; // namespace Standard
}; // namespace JamJar
#endif
#endif
