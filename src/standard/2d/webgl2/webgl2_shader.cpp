#ifdef __EMSCRIPTEN__
#include "standard/2d/webgl2/webgl2_shader.hpp"

JamJar::Standard::_2D::WebGL2Shader::WebGL2Shader(WebGL2ShaderType type, std::string name, std::string source)
    : type(type), name(name), source(source){};
#endif
