#ifndef STANDARD_2D_WEBGL2_WEBGL2_SYSTEM_HPP
#define STANDARD_2D_WEBGL2_WEBGL2_SYSTEM_HPP
#ifdef __EMSCRIPTEN__

#include "component/component.hpp"
#include "emscripten/html5.h"
#include "entity/entity.hpp"
#include "hash.hpp"
#include "message/listener.hpp"
#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "standard/2d/render/render_system.hpp"
#include "system/evaluator.hpp"

namespace JamJar::Standard::_2D {
class WebGL2System : public RenderSystem {
  private:
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE m_context{};
    static bool evaluator(Entity *entity, std::vector<JamJar::Component *> components);

  protected:
    void render(float deltaTime) override;

  public:
    explicit WebGL2System(MessageBus *messageBus, EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context);
};
}; // namespace JamJar::Standard::_2D

#endif
#endif
