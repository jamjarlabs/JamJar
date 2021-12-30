#include "rotator.hpp"
#include "entity/entity.hpp"
#include "game.hpp"
#include "geometry/polygon.hpp"
#include "geometry/vector_2d.hpp"
#include "message/message_payload.hpp"
#include "standard/2d/camera/camera.hpp"
#include "standard/2d/primitive/primitive.hpp"
#include "standard/2d/transform/transform.hpp"
#include "standard/file_texture/file_texture_request.hpp"
#include "standard/file_texture/file_texture_system.hpp"
#include "player.hpp"
#include <memory>

using JamJar::Color;
using JamJar::Entity;
using JamJar::Material;
using JamJar::Polygon;
using JamJar::Vector2D;
using JamJar::Standard::_2D::Camera;
using JamJar::Standard::_2D::Primitive;
using JamJar::Standard::_2D::Transform;

Rotator::Rotator(JamJar::MessageBus *messageBus) : Game(messageBus) {}

void Rotator::OnStart() {
    auto camera = new Entity(messageBus);
    camera->Add(new Transform());
    camera->Add(new Camera(Color(0, 0, 0)));

    auto player = new Entity(messageBus);
    player->Add(new Transform(Vector2D(0, 0), Vector2D(10, 10)));
    player->Add(new Primitive(Polygon({0, 0.5, 0.5, -0.5, -0.5, -0.5, 0, 0.5}), Material(Color(0, 1, 0, 1))));
    player->Add(new Player());
}
