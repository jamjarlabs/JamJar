#include "primitives.hpp"
#include "entity/entity.hpp"
#include "game.hpp"
#include "geometry/vector_2d.hpp"
#include "hash.hpp"
#include "message/message_payload.hpp"
#include "render/color.hpp"
#include "render/texture.hpp"
#include "standard/2d/camera/camera.hpp"
#include "standard/2d/primitive/primitive.hpp"
#include "standard/2d/transform/transform.hpp"
#include "standard/file_texture/file_texture_request.hpp"
#include "standard/file_texture/file_texture_system.hpp"
#include <cmath>
#include <memory>
#include <random>

const float PI = 3.14159265358979323846;

Primitives::Primitives(JamJar::MessageBus *messageBus) : Game(messageBus) {}

void Primitives::OnStart() {

    auto triangle = new JamJar::Entity(messageBus);
    triangle->Add(new JamJar::Standard::_2D::Transform(JamJar::Vector2D(30, 0), JamJar::Vector2D(30, 30)));
    triangle->Add(new JamJar::Standard::_2D::Primitive(JamJar::Polygon({0, 0.5, 0.5, -0.5, -0.5, -0.5, 0, 0.5}),
                                                       JamJar::Material(JamJar::Color(0, 1, 0, 1))));

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(3, 10);
    auto numberOfSides = uni(rng);

    std::vector<float> points;
    for (int i = 0; i < numberOfSides; i++) {
        points.push_back(cosf((2 * PI * i) / numberOfSides) / 2);
        points.push_back(sinf((2 * PI * i) / numberOfSides) / 2);
    }

    points.push_back(points[0]);
    points.push_back(points[1]);

    auto polygon = new JamJar::Entity(messageBus);
    polygon->Add(new JamJar::Standard::_2D::Transform(JamJar::Vector2D(-30, 0), JamJar::Vector2D(30, 30)));
    polygon->Add(
        new JamJar::Standard::_2D::Primitive(JamJar::Polygon(points), JamJar::Material(JamJar::Color(0, 1, 1, 1))));

    auto camera = new JamJar::Entity(messageBus);
    camera->Add(new JamJar::Standard::_2D::Transform());
    camera->Add(new JamJar::Standard::_2D::Camera(JamJar::Color(0, 0, 0)));
}
