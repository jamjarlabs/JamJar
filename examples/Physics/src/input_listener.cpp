#include "input_listener.hpp"
#include "entity/entity.hpp"
#include "message/message_payload.hpp"
#include "render/color.hpp"
#include "render/texture.hpp"
#include "standard/2d/box2d/box2d_body.hpp"
#include "standard/2d/camera/camera.hpp"
#include "standard/2d/primitive/primitive.hpp"
#include "standard/2d/transform/transform.hpp"
#include "standard/sdl2_input/sdl2_input_system.hpp"
#include <SDL2/SDL.h>
#include <cmath>
#include <memory>
#include <random>

const float PI = 3.14159265358979323846;

bool InputListener::evaluator(JamJar::Entity *entity, const std::vector<JamJar::Component *> &components) {
    for (const auto &component : components) {
        if (component->key == JamJar::Standard::_2D::Camera::KEY) {
            return true;
        }
    }
    return false;
}

InputListener::InputListener(JamJar::MessageBus *messageBus, SDL_Window *window)
    : JamJar::VectorSystem(messageBus, InputListener::evaluator), window(window) {
    this->messageBus->Subscribe(this, JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_BUTTON_DOWN_EVENT);
}

void InputListener::OnMessage(JamJar::Message *message) {
    VectorSystem::OnMessage(message);
    switch (message->type) {
    case JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_BUTTON_DOWN_EVENT: {
        auto *eventMessage = static_cast<JamJar::MessagePayload<JamJar::Standard::SDL2MouseEvent> *>(message);
        auto event = eventMessage->payload;

        if (this->entities.size() <= 0) {
            break;
        }

        auto cameraEntity = this->entities.front();
        auto transform = cameraEntity.Get<JamJar::Standard::_2D::Transform>();
        auto camera = cameraEntity.Get<JamJar::Standard::_2D::Camera>();

        auto worldPos = JamJar::Standard::_2D::MousePositionToWorldPosition(event.position, transform->position, camera,
                                                                            this->window);
        auto shape = new JamJar::Entity(messageBus);
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> uni(3, 8);
        auto numberOfSides = uni(rng);

        std::vector<float> points;
        std::vector<float> primitiveShape;
        for (int i = 0; i < numberOfSides; i++) {
            float x = cosf((2 * PI * i) / numberOfSides) / 2;
            float y = sinf((2 * PI * i) / numberOfSides) / 2;
            points.push_back(x);
            points.push_back(y);
            primitiveShape.push_back(x);
            primitiveShape.push_back(y);
        }

        primitiveShape.push_back(points[0]);
        primitiveShape.push_back(points[1]);

        shape->Add(new JamJar::Standard::_2D::Transform(worldPos, JamJar::Vector2D(10, 10)));
        shape->Add(new JamJar::Standard::_2D::Box2DBody(
            JamJar::Polygon(points),
            JamJar::Standard::_2D::Box2DBodyProperties({.density = 1.0f, .angularVelocity = 1.0f})));
        shape->Add(new JamJar::Standard::_2D::Primitive(JamJar::Polygon(primitiveShape),
                                                        JamJar::Material(JamJar::Color(0, 1, 0, 1))));
        break;
    }
    }
}
