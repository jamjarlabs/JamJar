#ifndef STANDARD_2D_SPRITE_WEBGL2_DEFAULT_SPRITE_SHADERS_HPP
#define STANDARD_2D_SPRITE_WEBGL2_DEFAULT_SPRITE_SHADERS_HPP
#ifdef __EMSCRIPTEN__

#include "standard/2d/webgl2/webgl2_shader.hpp"
#include <GLES3/gl3.h>
#include <optional>
#include <string>

namespace JamJar::Standard::_2D {

class WebGL2DefaultSpriteVertexShader : public WebGL2Shader {
  public:
    explicit WebGL2DefaultSpriteVertexShader();
    void PerProgram(WebGL2ShaderContext *context) override;
    void PerTexture(WebGL2ShaderContext *context, GLuint texture) override;
    void PerRenderable(WebGL2ShaderContext *context, Renderable renderable, GLuint texture) override;
};

class WebGL2DefaultSpriteFragmentShader : public WebGL2Shader {
  public:
    explicit WebGL2DefaultSpriteFragmentShader();
    void PerProgram(WebGL2ShaderContext *context) override;
    void PerTexture(WebGL2ShaderContext *context, GLuint texture) override;
    void PerRenderable(WebGL2ShaderContext *context, Renderable renderable, GLuint texture) override;
};
}; // namespace JamJar::Standard::_2D

#endif
#endif
