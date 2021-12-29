#include "physics_resizing.hpp"
#include "entity/entity.hpp"
#include "game.hpp"
#include "geometry/polygon.hpp"
#include "geometry/vector_2d.hpp"
#include "message/message_payload.hpp"
#include "render/color.hpp"
#include "render/texture.hpp"
#include "resizable.hpp"
#include "standard/2d/box2d/box2d_body.hpp"
#include "standard/2d/camera/camera.hpp"
#include "standard/2d/primitive/primitive.hpp"
#include "standard/2d/transform/transform.hpp"
#include "standard/file_texture/file_texture_request.hpp"
#include "standard/file_texture/file_texture_system.hpp"

PhysicsResizing::PhysicsResizing(JamJar::MessageBus *messageBus) : Game(messageBus) {}

void PhysicsResizing::OnStart() {

    auto resizable = new JamJar::Entity(messageBus);
    resizable->Add(new JamJar::Standard::_2D::Transform(JamJar::Vector2D(0, 0), JamJar::Vector2D(20, 20)));
    resizable->Add(new JamJar::Standard::_2D::Box2DBody(
        JamJar::Polygon({-0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, -0.5}),
        JamJar::Standard::_2D::Box2DBodyProperties({.density = 1.0f, .angularVelocity = 1.0f})));
    resizable->Add(
        new JamJar::Standard::_2D::Primitive(JamJar::Polygon({-0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5}),
                                             JamJar::Material(JamJar::Color(0, 1, 1, 1))));
    resizable->Add(new Resizable(10, 50));

    auto camera = new JamJar::Entity(messageBus);
    camera->Add(new JamJar::Standard::_2D::Transform());
    camera->Add(new JamJar::Standard::_2D::Camera(JamJar::Color(0, 0, 0)));
}
