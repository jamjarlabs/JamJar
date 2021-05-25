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
#include "standard/file_texture/file_texture_response.hpp"
#include "system/evaluator.hpp"
#include <GLES3/gl3.h>
#include <cstdint>
#include <unordered_map>

namespace JamJar::Standard::_2D {
class WebGL2System : public RenderSystem {
  private:
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE m_context{};
    std::unordered_map<std::uint32_t, GLuint> textures;
    static bool evaluator(Entity *entity, std::vector<JamJar::Component *> components);
    void loadTexture(FileTextureResponse *response);

  protected:
    void render(float deltaTime) override;

  public:
    explicit WebGL2System(MessageBus *messageBus, EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context);
    void OnMessage(JamJar::Message *message) override;
};
}; // namespace JamJar::Standard::_2D

#endif
#endif
