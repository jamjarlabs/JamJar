#ifdef __EMSCRIPTEN__
#include "standard/2d/sprite/webgl2_default_sprite_shaders.hpp"
#include "standard/2d/sprite/sprite_system.hpp"
#include <array>

const std::string VERTEX_SHADER_SOURCE = R"(#version 300 es
in vec2 aVertexPosition;
in vec2 aTexturePosition;

uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uProjectionMatrix;

out vec2 vTextureCoordinate;

void main() {
    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aVertexPosition, 0, 1);
    vTextureCoordinate = aTexturePosition;
})";

JamJar::Standard::_2D::WebGL2DefaultSpriteVertexShader::WebGL2DefaultSpriteVertexShader()
    : WebGL2Shader(WebGL2ShaderType::VERTEX, JamJar::Standard::_2D::SpriteSystem::DEFAULT_SPRITE_VERTEX_SHADER_NAME,
                   VERTEX_SHADER_SOURCE) {}

void JamJar::Standard::_2D::WebGL2DefaultSpriteVertexShader::PerProgram(WebGL2ShaderContext *context) {
    auto viewMatrix = Matrix4D();

    auto invertedPos = context->transform->position;
    invertedPos.x = -invertedPos.x;
    invertedPos.y = -invertedPos.y;

    viewMatrix.Translate(invertedPos);

    auto projectionMatrix = context->camera->ProjectionMatrix();

    // Supply camera matrices to GPU
    auto viewLocation = glGetUniformLocation(context->program, "uViewMatrix");
    auto projectionLocation = glGetUniformLocation(context->program, "uProjectionMatrix");

    glUniformMatrix4fv(viewLocation, 1, false, viewMatrix.data.data());
    glUniformMatrix4fv(projectionLocation, 1, false, projectionMatrix.data.data());
}

void JamJar::Standard::_2D::WebGL2DefaultSpriteVertexShader::PerTexture(WebGL2ShaderContext *context, GLuint texture) {}

void JamJar::Standard::_2D::WebGL2DefaultSpriteVertexShader::PerRenderable(WebGL2ShaderContext *context,
                                                                           Renderable renderable, GLuint texture) {
    auto modelLocation = glGetUniformLocation(context->program, "uModelMatrix");
    glUniformMatrix4fv(modelLocation, 1, false, renderable.modelMatrix.data.data());

    auto positionAttributeLocation = glGetAttribLocation(context->program, "aVertexPosition");
    auto textureAttributeLocation = glGetAttribLocation(context->program, "aTexturePosition");

    std::array<GLuint, 2> buffers;
    glGenBuffers(2, buffers.data());

    auto positionBuffer = buffers[0];
    auto textureBuffer = buffers[1];

    std::array<GLuint, 1> vaos;
    glGenVertexArrays(1, vaos.data());

    auto vao = vaos[0];

    glBindVertexArray(vao);

    // Position data
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(renderable.vertices[0]) * renderable.vertices.size(),
                 renderable.vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(positionAttributeLocation);

    glVertexAttribPointer(positionAttributeLocation, 2, GL_FLOAT, false, 0, 0);

    // Texture data
    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(renderable.material.texture->points[0]) * renderable.material.texture->points.size(),
                 renderable.material.texture->points.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(textureAttributeLocation);

    glVertexAttribPointer(textureAttributeLocation, 2, GL_FLOAT, false, 0, 0);

    std::array<float, 4> color({(float)renderable.material.color.red, (float)renderable.material.color.green,
                                (float)renderable.material.color.blue, (float)renderable.material.color.alpha});
}

const std::string FRAGMENT_SHADER_SOURCE = R"(#version 300 es
precision mediump float;

uniform sampler2D uTexture;
uniform vec4 uColor;

in vec2 vTextureCoordinate;

out vec4 outColor;
void main() {
    outColor = texture(uTexture, vTextureCoordinate);
    outColor = outColor * uColor;
})";

JamJar::Standard::_2D::WebGL2DefaultSpriteFragmentShader::WebGL2DefaultSpriteFragmentShader()
    : WebGL2Shader(WebGL2ShaderType::FRAGMENT, JamJar::Standard::_2D::SpriteSystem::DEFAULT_SPRITE_FRAGMENT_SHADER_NAME,
                   FRAGMENT_SHADER_SOURCE) {}

void JamJar::Standard::_2D::WebGL2DefaultSpriteFragmentShader::PerProgram(WebGL2ShaderContext *context) {}

void JamJar::Standard::_2D::WebGL2DefaultSpriteFragmentShader::PerTexture(WebGL2ShaderContext *context,
                                                                          GLuint texture) {
    auto textureLocation = glGetUniformLocation(context->program, "uTexture");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureLocation, 0);
}

void JamJar::Standard::_2D::WebGL2DefaultSpriteFragmentShader::PerRenderable(WebGL2ShaderContext *context,
                                                                             Renderable renderable, GLuint texture) {
    auto colorLocation = glGetUniformLocation(context->program, "uColor");
    std::array<float, 4> color({(float)renderable.material.color.red, (float)renderable.material.color.green,
                                (float)renderable.material.color.blue, (float)renderable.material.color.alpha});
    glUniform4fv(colorLocation, 1, color.data());
}

#endif
