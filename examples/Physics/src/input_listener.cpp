#include "input_listener.hpp"
#include "entity/entity.hpp"
#include "message/message_payload.hpp"
#include "render/color.hpp"
#include "render/texture.hpp"
#include "standard/2d/box2d/box2d_body.hpp"
#include "standard/2d/primitive/primitive.hpp"
#include "standard/2d/transform/transform.hpp"
#include "standard/sdl2_input/sdl2_input_system.hpp"
#include <SDL2/SDL.h>
#include <cmath>
#include <memory>
#include <random>

const float PI = 3.14159265358979323846;

InputListener::InputListener(JamJar::MessageBus *messageBus) : JamJar::System(messageBus) {
    this->messageBus->Subscribe(this, JamJar::Standard::SDL2InputSystem::MESSAGE_KEYBOARD_EVENT);
}

void InputListener::OnMessage(JamJar::Message *message) {
    System::OnMessage(message);
    switch (message->type) {
    case JamJar::Standard::SDL2InputSystem::MESSAGE_KEYBOARD_EVENT: {
        auto *eventMessage = static_cast<JamJar::MessagePayload<SDL_Event> *>(message);
        auto event = eventMessage->payload;
        if (event.key.type == SDL_KEYDOWN) {
            auto name = SDL_GetKeyName(event.key.keysym.sym);
            if (std::strcmp(name, "Space") == 0) {
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

                shape->Add(std::move(std::make_unique<JamJar::Standard::_2D::Transform>(JamJar::Vector2D(0, 30),
                                                                                        JamJar::Vector2D(10, 10))));
                shape->Add(std::move(std::make_unique<JamJar::Standard::_2D::Box2DBody>(
                    JamJar::Polygon(points),
                    JamJar::Standard::_2D::Box2DBodyProperties({.density = 1.0f, .angularVelocity = 1.0f}))));
                shape->Add(std::move(std::make_unique<JamJar::Standard::_2D::Primitive>(
                    JamJar::Polygon(primitiveShape), JamJar::Material(JamJar::Color(0, 1, 0, 1)))));
            }
        }
        break;
    }
    }
}
