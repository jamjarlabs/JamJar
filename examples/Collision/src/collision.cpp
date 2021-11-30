#include "collision.hpp"
#include "box.hpp"
#include "destructor.hpp"
#include "entity/entity.hpp"
#include "game.hpp"
#include "geometry/polygon.hpp"
#include "geometry/vector_2d.hpp"
#include "hash.hpp"
#include "message/message_payload.hpp"
#include "render/color.hpp"
#include "render/texture.hpp"
#include "standard/2d/box2d/box2d_body.hpp"
#include "standard/2d/camera/camera.hpp"
#include "standard/2d/primitive/primitive.hpp"
#include "standard/2d/transform/transform.hpp"
#include "standard/file_texture/file_texture_request.hpp"
#include "standard/file_texture/file_texture_system.hpp"
#include <cmath>
#include <memory>
#include <random>

const float PI = 3.14159265358979323846;

Collision::Collision(JamJar::MessageBus *messageBus) : Game(messageBus) {}

void Collision::OnStart() {

    auto ramp = new JamJar::Entity(messageBus);
    ramp->Add(std::move(
        std::make_unique<JamJar::Standard::_2D::Transform>(JamJar::Vector2D(-10, 0), JamJar::Vector2D(130, 80))));
    ramp->Add(std::move(std::make_unique<JamJar::Standard::_2D::Box2DBody>(
        JamJar::Polygon({-0.5, 0.5, 0.5, -0.5, -0.5, -0.5}),
        JamJar::Standard::_2D::Box2DBodyProperties({.bodyType = JamJar::Standard::_2D::Box2DBodyType::STATIC}))));
    ramp->Add(std::move(std::make_unique<JamJar::Standard::_2D::Primitive>(
        JamJar::Polygon({-0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5}), JamJar::Material(JamJar::Color(1, 1, 1, 1)))));

    auto destroyer = new JamJar::Entity(messageBus);
    destroyer->Add(std::move(
        std::make_unique<JamJar::Standard::_2D::Transform>(JamJar::Vector2D(60, 0), JamJar::Vector2D(10, 80))));
    destroyer->Add(std::move(std::make_unique<JamJar::Standard::_2D::Box2DBody>(
        JamJar::Polygon({-0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, -0.5}),
        JamJar::Standard::_2D::Box2DBodyProperties(
            {.bodyType = JamJar::Standard::_2D::Box2DBodyType::STATIC, .isSensor = true}))));
    destroyer->Add(std::move(std::make_unique<JamJar::Standard::_2D::Primitive>(
        JamJar::Polygon({-0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5}),
        JamJar::Material(JamJar::Color(1, 0, 0, 1)))));
    destroyer->Add(std::move(std::make_unique<Destructor>()));

    auto camera = new JamJar::Entity(messageBus);
    camera->Add(std::move(std::make_unique<JamJar::Standard::_2D::Transform>()));
    camera->Add(std::move(std::make_unique<JamJar::Standard::_2D::Camera>(JamJar::Color(0, 0, 0))));
}
