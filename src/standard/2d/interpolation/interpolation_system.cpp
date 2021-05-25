#include "standard/2d/interpolation/interpolation_system.hpp"
#include "component/component.hpp"
#include "entity/entity.hpp"
#include "game.hpp"
#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "standard/2d/transform/transform.hpp"

JamJar::Standard::_2D::InterpolationSystem::InterpolationSystem(MessageBus *messageBus)
    : MapSystem(messageBus, JamJar::Standard::_2D::InterpolationSystem::evaluator) {
    this->messageBus->Subscribe(this, JamJar::Game::MESSAGE_POST_RENDER);
}

bool hasTransform(JamJar::Component *component) { return component->key == JamJar::Standard::_2D::Transform::KEY; }

bool JamJar::Standard::_2D::InterpolationSystem::evaluator(Entity *entity,
                                                           const std::vector<JamJar::Component *> &components) {
    return std::any_of(components.begin(), components.end(), hasTransform);
}

void JamJar::Standard::_2D::InterpolationSystem::OnMessage(JamJar::Message *message) {
    MapSystem::OnMessage(message);
    switch (message->type) {
    case JamJar::Game::MESSAGE_POST_RENDER: {
        this->interpolate();
        break;
    }
    }
}

void JamJar::Standard::_2D::InterpolationSystem::interpolate() {
    for (const auto &entityPair : this->entities) {
        auto entity = entityPair.second;
        auto *transform =
            static_cast<JamJar::Standard::_2D::Transform *>(entity.Get(JamJar::Standard::_2D::Transform::KEY));

        transform->previous = transform->position;
    }
}
