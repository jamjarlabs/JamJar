#ifdef __EMSCRIPTEN__
#include "standard/2d/webgl2/webgl2_system.hpp"
#include "component/component.hpp"
#include "emscripten/html5.h"
#include "entity/entity.hpp"
#include "geometry/matrix_4d.hpp"
#include "message/message_bus.hpp"
#include "message/message_payload.hpp"
#include "standard/2d/camera/camera.hpp"
#include "standard/2d/transform/transform.hpp"
#include "standard/2d/webgl2/webgl2_shader.hpp"
#include "standard/file_texture/file_texture_response.hpp"
#include "standard/file_texture/file_texture_system.hpp"
#include <GLES3/gl3.h>
#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

const unsigned char DEFAULT_TEXTURE[4] = {255, 255, 255, 255};

const std::unordered_map<JamJar::TextureFilter, int> FILTERS = {
    {JamJar::TextureFilter::NEAREST, GL_NEAREST},
    {JamJar::TextureFilter::BILINEAR, GL_LINEAR},
    {JamJar::TextureFilter::TRILINEAR, GL_LINEAR_MIPMAP_LINEAR}};

const std::unordered_map<JamJar::TextureWrap, int> WRAPS = {{JamJar::TextureWrap::REPEAT, GL_REPEAT},
                                                            {JamJar::TextureWrap::MIRRORED_REPEAT, GL_MIRRORED_REPEAT},
                                                            {JamJar::TextureWrap::CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE}};

const std::unordered_map<JamJar::Standard::_2D::DrawMode, int> DRAW_MODES = {
    {JamJar::Standard::_2D::DrawMode::POINTS, GL_POINTS},
    {JamJar::Standard::_2D::DrawMode::LINES, GL_LINES},
    {JamJar::Standard::_2D::DrawMode::LINE_STRIP, GL_LINE_STRIP},
    {JamJar::Standard::_2D::DrawMode::TRIANGLES, GL_TRIANGLES},
    {JamJar::Standard::_2D::DrawMode::TRIANGLE_STRIP, GL_TRIANGLE_STRIP}};

const std::unordered_map<JamJar::Standard::_2D::WebGL2ShaderType, GLenum> SHADER_TYPES = {
    {JamJar::Standard::_2D::WebGL2ShaderType::VERTEX, GL_VERTEX_SHADER},
    {JamJar::Standard::_2D::WebGL2ShaderType::FRAGMENT, GL_FRAGMENT_SHADER}};

JamJar::Standard::_2D::WebGL2System::WebGL2System(MessageBus *messageBus, EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context)
    : RenderSystem(messageBus, WebGL2System::evaluator), m_context(context), m_defaultTexture(JamJar::Texture(0)) {
    this->messageBus->Subscribe(this, JamJar::Standard::FileTextureSystem::MESSAGE_RESPONSE_FILE_TEXTURE_LOAD);
    this->messageBus->Subscribe(this, JamJar::Standard::_2D::WebGL2System::MESSAGE_LOAD_SHADER);

    this->m_defaultTextureRef = this->createTexture(1, 1,
                                                    JamJar::TextureProperties({
                                                        .xWrap = JamJar::TextureWrap::REPEAT,
                                                        .yWrap = JamJar::TextureWrap::REPEAT,
                                                        .minFilter = JamJar::TextureFilter::NEAREST,
                                                        .magFilter = JamJar::TextureFilter::NEAREST,
                                                    }),
                                                    DEFAULT_TEXTURE);
}

bool JamJar::Standard::_2D::WebGL2System::evaluator(Entity *entity, std::vector<JamJar::Component *> components) {
    bool hasCamera = false;
    bool hasTransform = false;
    for (const auto &component : components) {
        if (component->key == JamJar::Standard::_2D::Camera::KEY) {
            hasCamera = true;
        }
        if (component->key == JamJar::Standard::_2D::Transform::KEY) {
            hasTransform = true;
        }
        if (hasCamera && hasTransform) {
            return true;
        }
    }
    return false;
}

void JamJar::Standard::_2D::WebGL2System::OnMessage(JamJar::Message *message) {
    RenderSystem::OnMessage(message);
    switch (message->type) {
    case JamJar::Standard::_2D::RenderSystem::MESSAGE_LOAD_RENDERABLES: {
        auto *loadMessage = static_cast<JamJar::MessagePayload<std::vector<Renderable>> *>(message);
        this->renderables.insert(this->renderables.end(), loadMessage->payload.begin(), loadMessage->payload.end());
        break;
    }
    case JamJar::Standard::FileTextureSystem::MESSAGE_RESPONSE_FILE_TEXTURE_LOAD: {
        auto *loadMessage =
            static_cast<JamJar::MessagePayload<std::unique_ptr<JamJar::Standard::FileTextureResponse>> *>(message);
        this->loadTexture(loadMessage->payload.get());
        break;
    }
    case JamJar::Standard::_2D::WebGL2System::MESSAGE_LOAD_SHADER: {
        auto *loadMessage =
            static_cast<JamJar::MessagePayload<std::unique_ptr<JamJar::Standard::_2D::WebGL2Shader>> *>(message);
        this->loadShader(std::move(loadMessage->payload));
    }
    }
}

void JamJar::Standard::_2D::WebGL2System::loadTexture(FileTextureResponse *response) {

    if (response->status != FileTextureResponseStatus::SUCCESS) {
        // Don't try to process any responses that aren't successful
        return;
    }

    auto properties = response->request->properties;

    GLuint texture = this->createTexture(response->width, response->height, response->request->properties,
                                         response->data.get()->data());
    this->textures.insert({response->request->key, texture});
}

GLuint JamJar::Standard::_2D::WebGL2System::createTexture(const int width, const int height,
                                                          JamJar::TextureProperties properties, const void *data) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WRAPS.at(properties.xWrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WRAPS.at(properties.yWrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FILTERS.at(properties.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FILTERS.at(properties.magFilter));
    if (properties.generateMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    glBindTexture(GL_TEXTURE_2D, NULL);
    return texture;
}

void JamJar::Standard::_2D::WebGL2System::loadShader(std::unique_ptr<WebGL2Shader> shader) {
    auto glShader = glCreateShader(SHADER_TYPES.at(shader->type));
    GLchar const *files[] = {shader->source.c_str()};
    GLint lengths[] = {GLint(shader->source.size())};
    glShaderSource(glShader, 1, files, lengths);
    glCompileShader(glShader);
    GLint compileSuccess = 0;
    glGetShaderiv(glShader, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE) {
        // fail to compile shader!
        glDeleteShader(glShader);
        throw std::exception();
    }
    auto loadedShader =
        std::make_unique<WebGL2LoadedShader>(WebGL2LoadedShader({.definition = std::move(shader), .shader = glShader}));

    this->m_shaders.insert({loadedShader->definition->name, std::move(loadedShader)});
}

GLuint JamJar::Standard::_2D::WebGL2System::getProgram(std::string identifier, GLuint fragment, GLuint vertex) {
    if (this->m_programs.count(identifier) == 0) {
        // No program, create it
        auto program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);
        GLint linkSuccess = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
        if (linkSuccess == GL_FALSE) {
            // fail to compile program
            glDeleteProgram(program);
            throw std::exception();
        }
        this->m_programs.insert({identifier, program});
        return program;
    } else {
        return this->m_programs.at(identifier);
    }
}

void JamJar::Standard::_2D::WebGL2System::render(float deltaTime) {
    for (const auto &entityPair : this->entities) {
        auto cameraEntity = entityPair.second;

        auto camera =
            static_cast<JamJar::Standard::_2D::Camera *>(cameraEntity.Get(JamJar::Standard::_2D::Camera::KEY));
        auto transform =
            static_cast<JamJar::Standard::_2D::Transform *>(cameraEntity.Get(JamJar::Standard::_2D::Transform::KEY));

        float canvasWidth = 1024;
        float canvasHeight = 576;

        // realWidth and realHeight are the width and height of the viewport
        // relative to the canvas with and height, rather than the normalised
        // scale of viewportScale
        auto realWidth = canvasWidth * camera->viewportScale.x;
        auto realHeight = canvasHeight * camera->viewportScale.y;

        // realPosition is the center position of the camera viewport in relation to
        // the canvas converted from the -1 to +1 coordinates of the viewportPosition
        // combined with the real width and height to make sure it is in the center
        // of the viewport.
        auto realPositionX = canvasWidth / 2 + (camera->viewportPosition.x / 2) * canvasWidth - realWidth / 2;
        auto realPositionY = canvasHeight / 2 + (camera->viewportPosition.y / 2) * canvasHeight - realHeight / 2;

        // Define the viewport position of the camera
        glViewport(realPositionX, realPositionY, realWidth, realHeight);

        // Define scissor around camera viewport, ensures that nothing is rendered outside
        // of the viewport defined for this camera
        glScissor(realPositionX, realPositionY, realWidth, realHeight);

        glEnable(GL_SCISSOR_TEST);

        glClearDepthf(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set the background color
        glClearColor(camera->backgroundColor.red, camera->backgroundColor.green, camera->backgroundColor.blue,
                     camera->backgroundColor.alpha);

        // Enable depth testing (objects can appear behind/infront of eachother)
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        // Enable alpha blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        std::vector<Renderable> cameraRenderables = this->renderables;

        if (cameraRenderables.size() == 0) {
            // No renderables prepared for camera
            continue;
        }

        for (auto renderable : cameraRenderables) {
            std::string vertexShader = renderable.material.shaders->vertexShader;
            std::string fragmentShader = renderable.material.shaders->fragmentShader;

            if (this->m_shaders.count(fragmentShader) == 0) {
                continue;
            }

            if (this->m_shaders.count(vertexShader) == 0) {
                continue;
            }

            auto vertShader = this->m_shaders.at(vertexShader).get();
            auto fragShader = this->m_shaders.at(fragmentShader).get();

            auto programName = vertShader->definition->name + "_" + fragShader->definition->name;

            auto program = this->getProgram(programName, fragShader->shader, vertShader->shader);

            glUseProgram(program);

            auto context =
                std::make_unique<JamJar::Standard::_2D::WebGL2ShaderContext>(JamJar::Standard::_2D::WebGL2ShaderContext(
                    {.program = program, .camera = camera, .transform = transform}));

            vertShader->definition->PerProgram(context.get());
            fragShader->definition->PerProgram(context.get());

            GLuint textureRef = this->m_defaultTextureRef;
            JamJar::Texture texture = this->m_defaultTexture;

            if (renderable.material.texture.has_value()) {
                textureRef = this->textures[renderable.material.texture->image];
                texture = *renderable.material.texture;
            }

            vertShader->definition->PerTexture(context.get(), &texture, textureRef);
            fragShader->definition->PerTexture(context.get(), &texture, textureRef);

            vertShader->definition->PerRenderable(context.get(), &texture, &renderable, textureRef);
            fragShader->definition->PerRenderable(context.get(), &texture, &renderable, textureRef);

            glDrawArrays(DRAW_MODES.at(renderable.drawMode), 0, renderable.vertices.size() / 2);
        }
    }

    this->renderables.clear();
}
#endif
