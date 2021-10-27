#ifndef STANDARD_2D_PRIMITIVE_WEBGL2_DEFAULT_PRIMITIVE_SHADERS_HPP
#define STANDARD_2D_PRIMITIVE_WEBGL2_DEFAULT_PRIMITIVE_SHADERS_HPP
#ifdef __EMSCRIPTEN__

#include "standard/2d/webgl2/webgl2_shader.hpp"
#include <GLES3/gl3.h>

namespace JamJar::Standard::_2D {

class WebGL2DefaultPrimitiveVertexShader : public WebGL2Shader {
  public:
    explicit WebGL2DefaultPrimitiveVertexShader();
    void PerProgram(WebGL2ShaderContext *context) override;
    void PerTexture(WebGL2ShaderContext *context, JamJar::Texture *texture, GLuint textureRef) override;
    void PerRenderable(WebGL2ShaderContext *context, JamJar::Texture *texture, Renderable *renderable,
                       GLuint textureRef) override;
};

class WebGL2DefaultPrimitiveFragmentShader : public WebGL2Shader {
  public:
    explicit WebGL2DefaultPrimitiveFragmentShader();
    void PerProgram(WebGL2ShaderContext *context) override;
    void PerTexture(WebGL2ShaderContext *context, JamJar::Texture *texture, GLuint textureRef) override;
    void PerRenderable(WebGL2ShaderContext *context, JamJar::Texture *texture, Renderable *renderable,
                       GLuint textureRef) override;
};
}; // namespace JamJar::Standard::_2D

#endif
#endif
