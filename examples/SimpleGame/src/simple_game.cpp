#include "simple_game.hpp"
#include "entity/entity.hpp"
#include "game.hpp"
#include "geometry/vector_2d.hpp"
#include "render/color.hpp"
#include "standard/2d/camera/camera.hpp"
#include "standard/2d/motion/motion.hpp"
#include "standard/2d/primitive/primitive.hpp"
#include "standard/2d/transform/transform.hpp"
#include <memory>

SimpleGame::SimpleGame(JamJar::MessageBus *messageBus) : Game(messageBus) {}

void SimpleGame::OnStart() {
    std::vector<float> vec = {
        0, 0, 0, 30, 30, 0,
    };
    auto entity = new JamJar::Entity(m_messageBus);
    entity->Add(std::move(
        std::make_unique<JamJar::Standard::_2D::Transform>(JamJar::Vector2D(1, 1), JamJar::Vector2D(0.5, 0.5))));
    entity->Add(std::move(std::make_unique<JamJar::Standard::_2D::Motion>(JamJar::Vector2D(20, 0))));
    entity->Add(std::move(std::make_unique<JamJar::Standard::_2D::Primitive>(vec)));

    auto camera = new JamJar::Entity(m_messageBus);
    camera->Add(std::move(std::make_unique<JamJar::Standard::_2D::Transform>()));
    camera->Add(std::move(std::make_unique<JamJar::Standard::_2D::Camera>(JamJar::Color(0, 0, 0))));
}
