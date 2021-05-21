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
#include "standard/file_texture/file_texture_response.hpp"
#include "standard/file_texture/file_texture_system.hpp"
#include <GLES3/gl3.h>
#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

const std::string vertShader = R"(#version 300 es
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

const std::string fragShader = R"(#version 300 es
precision mediump float;

uniform sampler2D uTexture;
uniform vec4 uColor;

in vec2 vTextureCoordinate;

out vec4 outColor;
void main() {
    outColor = texture(uTexture, vTextureCoordinate);
    outColor = outColor * uColor;
})";

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

JamJar::Standard::_2D::WebGL2System::WebGL2System(MessageBus *messageBus, EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context)
    : RenderSystem(messageBus, WebGL2System::evaluator), m_context(context) {
    this->messageBus->Subscribe(this, JamJar::Standard::FileTextureSystem::MESSAGE_RESPONSE_FILE_TEXTURE_LOAD);
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
    }
}

void JamJar::Standard::_2D::WebGL2System::loadTexture(FileTextureResponse *response) {

    if (response->status != FileTextureResponseStatus::SUCCESS) {
        // Don't try to process any responses that aren't successful
        return;
    }

    auto properties = response->request->properties;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, response->width, response->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 response->data.get()->data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WRAPS.at(properties.xWrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WRAPS.at(properties.yWrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FILTERS.at(properties.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FILTERS.at(properties.magFilter));
    if (properties.generateMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    glBindTexture(GL_TEXTURE_2D, NULL);
    this->textures.insert({response->request->key, texture});
}

GLuint createShader(GLenum type, std::string source) {
    auto shader = glCreateShader(type);
    GLchar const *files[] = {source.c_str()};
    GLint lengths[] = {GLint(source.size())};
    glShaderSource(shader, 1, files, lengths);
    glCompileShader(shader);
    GLint compileSuccess = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE) {
        // fail to compile shader!
        glDeleteShader(shader);
        throw std::exception();
    }

    return shader;
}

GLuint createProgram(GLuint vertexShader, GLuint fragmentShader) {
    auto program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    GLint linkSuccess = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        // fail to compile shader!
        glDeleteProgram(program);
        throw std::exception();
    }
    return program;
}

void JamJar::Standard::_2D::WebGL2System::render(float deltaTime) {
    auto vertexShader = createShader(GL_VERTEX_SHADER, vertShader);
    auto fragmentShader = createShader(GL_FRAGMENT_SHADER, fragShader);

    auto program = createProgram(vertexShader, fragmentShader);

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

        glUseProgram(program);

        auto viewMatrix = Matrix4D();

        auto invertedPos = transform->position;
        invertedPos.x = -invertedPos.x;
        invertedPos.y = -invertedPos.y;

        viewMatrix.Translate(invertedPos);

        auto projectionMatrix = camera->ProjectionMatrix();

        // Supply camera matrices to GPU
        auto viewLocation = glGetUniformLocation(program, "uViewMatrix");
        auto projectionLocation = glGetUniformLocation(program, "uProjectionMatrix");

        glUniformMatrix4fv(viewLocation, 1, false, viewMatrix.data.data());
        glUniformMatrix4fv(projectionLocation, 1, false, projectionMatrix.data.data());

        for (auto renderable : cameraRenderables) {
            if (this->textures.count(renderable.material.texture->image) == 0) {
                continue;
            }
            auto modelLocation = glGetUniformLocation(program, "uModelMatrix");

            glUniformMatrix4fv(modelLocation, 1, false, renderable.modelMatrix.data.data());

            auto textureLocation = glGetUniformLocation(program, "uTexture");

            auto tex = this->textures[renderable.material.texture->image];

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex);
            glUniform1i(textureLocation, 0);

            auto positionAttributeLocation = glGetAttribLocation(program, "aVertexPosition");
            auto textureAttributeLocation = glGetAttribLocation(program, "aTexturePosition");

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

            // Prepare frag shader
            auto colorLocation = glGetUniformLocation(program, "uColor");
            glUniform4fv(colorLocation, 1, color.data());

            glDrawArrays(DRAW_MODES.at(renderable.drawMode), 0, renderable.vertices.size() / 2);
        }
    }

    this->renderables.clear();
}
#endif
