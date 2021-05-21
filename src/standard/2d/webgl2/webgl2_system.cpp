#ifdef __EMSCRIPTEN__
#include "standard/2d/webgl2/webgl2_system.hpp"
#include "component/component.hpp"
#include "emscripten/html5.h"
#include "entity/entity.hpp"
#include "geometry/matrix_4d.hpp"
#include "message/message_bus.hpp"
#include "standard/2d/camera/camera.hpp"
#include "standard/2d/transform/transform.hpp"
#include <GLES3/gl3.h>
#include <array>
#include <iostream>
#include <string>

const std::string vertShader = R"(#version 300 es
in vec2 aVertexPosition;

uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uProjectionMatrix;

void main() {
    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aVertexPosition, 0, 1);
})";

const std::string fragShader = R"(#version 300 es
precision mediump float;
uniform vec4 uColor;
out vec4 outColor;
void main() {
    outColor = uColor;
})";

JamJar::Standard::_2D::WebGL2System::WebGL2System(MessageBus *messageBus, EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context)
    : RenderSystem(messageBus, WebGL2System::evaluator), m_context(context) {}

// NOLINTNEXTLINE(misc-unused-parameters)
bool JamJar::Standard::_2D::WebGL2System::evaluator(Entity *entity, std::vector<JamJar::Component *> components) {
    bool hasCamera = false;
    bool hasTransform = false;
    for (const auto &component : components) {
        if (component->m_key == JamJar::Standard::_2D::Camera::CAMERA_KEY) {
            hasCamera = true;
        }
        if (component->m_key == JamJar::Standard::_2D::Transform::TRANSFORM_KEY) {
            hasTransform = true;
        }
        if (hasCamera && hasTransform) {
            return true;
        }
    }
    return false;
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

    for (const auto &entityPair : this->m_entities) {
        auto cameraEntity = entityPair.second;

        auto camera =
            static_cast<JamJar::Standard::_2D::Camera *>(cameraEntity.Get(JamJar::Standard::_2D::Camera::CAMERA_KEY));
        auto transform = static_cast<JamJar::Standard::_2D::Transform *>(
            cameraEntity.Get(JamJar::Standard::_2D::Transform::TRANSFORM_KEY));

        float canvasWidth = 1024;
        float canvasHeight = 576;

        // realWidth and realHeight are the width and height of the viewport
        // relative to the canvas with and height, rather than the normalised
        // scale of viewportScale
        auto realWidth = canvasWidth * camera->m_viewportScale.m_x;
        auto realHeight = canvasHeight * camera->m_viewportScale.m_y;

        // realPosition is the center position of the camera viewport in relation to
        // the canvas converted from the -1 to +1 coordinates of the viewportPosition
        // combined with the real width and height to make sure it is in the center
        // of the viewport.
        auto realPositionX = canvasWidth / 2 + (camera->m_viewportPosition.m_x / 2) * canvasWidth - realWidth / 2;
        auto realPositionY = canvasHeight / 2 + (camera->m_viewportPosition.m_y / 2) * canvasHeight - realHeight / 2;

        // Define the viewport position of the camera
        glViewport(realPositionX, realPositionY, realWidth, realHeight);

        // Define scissor around camera viewport, ensures that nothing is rendered outside
        // of the viewport defined for this camera
        glScissor(realPositionX, realPositionY, realWidth, realHeight);

        glEnable(GL_SCISSOR_TEST);

        glClearDepthf(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set the background color
        glClearColor(camera->m_backgroundColor.m_red, camera->m_backgroundColor.m_green,
                     camera->m_backgroundColor.m_blue, camera->m_backgroundColor.m_alpha);

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

        auto invertedPos = transform->m_position;
        invertedPos.m_x = -invertedPos.m_x;
        invertedPos.m_y = -invertedPos.m_y;

        viewMatrix.Translate(invertedPos);

        auto projectionMatrix = camera->ProjectionMatrix();

        // Supply camera matrices to GPU
        auto viewLocation = glGetUniformLocation(program, "uViewMatrix");
        auto projectionLocation = glGetUniformLocation(program, "uProjectionMatrix");

        glUniformMatrix4fv(viewLocation, 1, false, viewMatrix.m_data.data());
        glUniformMatrix4fv(projectionLocation, 1, false, projectionMatrix.m_data.data());

        for (auto renderable : cameraRenderables) {
            auto modelLocation = glGetUniformLocation(program, "uModelMatrix");

            glUniformMatrix4fv(modelLocation, 1, false, renderable.m_modelMatrix.m_data.data());

            auto positionAttributeLocation = glGetAttribLocation(program, "aVertexPosition");

            std::array<GLuint, 1> buffers;
            glGenBuffers(1, buffers.data());

            auto positionBuffer = buffers[0];

            glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);

            // std::array<float, 6> positions({
            //     0, 0, 0, 0.5, 0.7, 0,
            // });

            glBufferData(GL_ARRAY_BUFFER, sizeof(renderable.m_vertices[0]) * renderable.m_vertices.size(),
                         renderable.m_vertices.data(), GL_STATIC_DRAW);

            std::array<GLuint, 1> vaos;
            glGenVertexArrays(1, vaos.data());

            auto vao = vaos[0];

            glBindVertexArray(vao);

            glEnableVertexAttribArray(positionAttributeLocation);

            glVertexAttribPointer(positionAttributeLocation, 2, GL_FLOAT, false, 0, 0);

            glBindVertexArray(vao);

            std::array<float, 4> color({1.0, 0.0, 0.0, 1.0});

            // Prepare frag shader
            auto colorLocation = glGetUniformLocation(program, "uColor");
            glUniform4fv(colorLocation, 1, color.data());

            glDrawArrays(GL_TRIANGLES, 0, renderable.m_vertices.size() / 2);
        }
    }

    this->renderables.clear();
}
#endif
