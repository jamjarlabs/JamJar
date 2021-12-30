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
#include "render/texture.hpp"
#include "standard/2d/render/render_system.hpp"
#include "standard/2d/webgl2/webgl2_shader.hpp"
#include "standard/file_texture/file_texture_response.hpp"
#include <GLES3/gl3.h>
#include <SDL2/SDL.h>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace JamJar::Standard::_2D {
struct WebGL2LoadedShader {
    std::unique_ptr<WebGL2Shader> definition;
    GLuint shader;
};

class WebGL2System : public RenderSystem {
  private:
    SDL_Window *window;
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE m_context{};
    std::unordered_map<std::uint32_t, GLuint> textures;
    GLuint m_defaultTextureRef;
    JamJar::Texture m_defaultTexture;
    std::unordered_map<std::string, std::unique_ptr<WebGL2LoadedShader>> m_shaders;
    std::unordered_map<std::string, GLuint> m_programs;
    static bool evaluator(Entity *entity, std::vector<JamJar::Component *> components);
    void loadTexture(FileTextureResponse *response);
    GLuint createTexture(const int width, const int height, JamJar::TextureProperties properties, const void *data);
    void loadShader(std::unique_ptr<WebGL2Shader> shader);
    GLuint getProgram(std::string identifier, GLuint fragment, GLuint vertex);

  protected:
    void render(float deltaTime) override;

  public:
    constexpr static uint32_t MESSAGE_LOAD_SHADER = hash("jamjar_2d_webgl2_load_shader");
    explicit WebGL2System(MessageBus *messageBus, SDL_Window *window, EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context);
    void OnMessage(JamJar::Message *message) override;
};
}; // namespace JamJar::Standard::_2D

#endif
#endif
