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

using JamJar::Color;
using JamJar::Entity;
using JamJar::Material;
using JamJar::Polygon;
using JamJar::Vector2D;
using JamJar::Standard::_2D::Box2DBody;
using JamJar::Standard::_2D::Box2DBodyProperties;
using JamJar::Standard::_2D::Box2DBodyType;
using JamJar::Standard::_2D::Camera;
using JamJar::Standard::_2D::Primitive;
using JamJar::Standard::_2D::Transform;

const float PI = 3.14159265358979323846;

Collision::Collision(JamJar::MessageBus *messageBus) : Game(messageBus) {}

void Collision::OnStart() {

    auto ramp = new Entity(messageBus);
    ramp->Add(new Transform(Vector2D(-10, 0), JamJar::Vector2D(130, 80)));
    ramp->Add(new Box2DBody(Polygon({-0.5, 0.5, 0.5, -0.5, -0.5, -0.5}),
                            Box2DBodyProperties({.bodyType = Box2DBodyType::STATIC})));
    ramp->Add(new Primitive(Polygon({-0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5}), Material(Color(1, 1, 1, 1))));

    auto destroyer = new Entity(messageBus);
    destroyer->Add(new Transform(Vector2D(60, 0), Vector2D(10, 80)));
    destroyer->Add(new Box2DBody(Polygon({-0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, -0.5}),
                                 Box2DBodyProperties({.bodyType = Box2DBodyType::STATIC, .isSensor = true})));
    destroyer->Add(
        new Primitive(Polygon({-0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5}), Material(Color(1, 0, 0, 1))));
    destroyer->Add(new Destructor());

    auto camera = new Entity(messageBus);
    camera->Add(new Transform());
    camera->Add(new Camera(Color(0, 0, 0)));
}
