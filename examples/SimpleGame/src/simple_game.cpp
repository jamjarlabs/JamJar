#include "simple_game.hpp"
#include "entity/entity.hpp"
#include "game.hpp"
#include "geometry/vector_2d.hpp"
#include "render/color.hpp"
#include "standard/camera/camera.hpp"
#include "standard/motion/motion.hpp"
#include "standard/primitive/primitive.hpp"
#include "standard/transform/transform.hpp"
#include <memory>

SimpleGame::SimpleGame(JamJar::MessageBus *messageBus) : Game(messageBus) {}

void SimpleGame::OnStart() {
    std::vector<float> vec = {
        0, 0, 0, 30, 30, 0,
    };
    auto entity = new JamJar::Entity(m_messageBus);
    entity->Add(
        std::move(std::make_unique<JamJar::Standard::Transform>(JamJar::Vector2D(1, 1), JamJar::Vector2D(0.5, 0.5))));
    entity->Add(std::move(std::make_unique<JamJar::Standard::Motion>(JamJar::Vector2D(20, 0))));
    entity->Add(std::move(std::make_unique<JamJar::Standard::Primitive>(vec)));

    auto camera = new JamJar::Entity(m_messageBus);
    camera->Add(std::move(std::make_unique<JamJar::Standard::Transform>()));
    camera->Add(std::move(std::make_unique<JamJar::Standard::Camera>(JamJar::Color(0, 0, 0))));
}
