#ifndef STANDARD_2D_WEBGL2_WEBGL2_SHADER_HPP
#define STANDARD_2D_WEBGL2_WEBGL2_SHADER_HPP
#ifdef __EMSCRIPTEN__

#include "standard/2d/camera/camera.hpp"
#include "standard/2d/render/renderable.hpp"
#include "standard/2d/transform/transform.hpp"
#include <GLES3/gl3.h>
#include <optional>
#include <string>

namespace JamJar::Standard::_2D {

enum WebGL2ShaderType { VERTEX, FRAGMENT };

struct WebGL2ShaderContext {
    GLuint program;
    Camera *camera;
    Transform *transform;
};

class WebGL2Shader {
  public:
    explicit WebGL2Shader(WebGL2ShaderType type, std::string name, std::string source);
    WebGL2ShaderType type;
    std::string name;
    std::string source;
    virtual void PerProgram(WebGL2ShaderContext *context) = 0;
    virtual void PerTexture(WebGL2ShaderContext *context, GLuint texture) = 0;
    virtual void PerRenderable(WebGL2ShaderContext *context, Renderable renderable, GLuint texture) = 0;
    virtual ~WebGL2Shader(){};
};
}; // namespace JamJar::Standard::_2D

#endif
#endif
