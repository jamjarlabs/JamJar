#include "standard/2d/primitive/primitive_system.hpp"
#include "component/component.hpp"
#include "entity/entity.hpp"
#include "game.hpp"
#include "message/message_bus.hpp"
#include "message/message_payload.hpp"
#include "standard/2d/primitive/primitive.hpp"
#include "standard/2d/render/render_system.hpp"
#include "standard/2d/render/renderable.hpp"
#include "standard/2d/transform/transform.hpp"

JamJar::Standard::_2D::PrimitiveSystem::PrimitiveSystem(MessageBus *messageBus)
    : MapSystem(messageBus, JamJar::Standard::_2D::PrimitiveSystem::evaluator) {
    this->m_messageBus->Subscribe(this, JamJar::Game::MESSAGE_PRE_RENDER);
}

// NOLINTNEXTLINE(misc-unused-parameters)
bool JamJar::Standard::_2D::PrimitiveSystem::evaluator(Entity *entity,
                                                       const std::vector<JamJar::Component *> &components) {
    bool hasPrimitive = false;
    bool hasTransform = false;
    for (const auto &component : components) {
        if (component->m_key == JamJar::Standard::_2D::Primitive::PRIMITIVE_KEY) {
            hasPrimitive = true;
        }
        if (component->m_key == JamJar::Standard::_2D::Transform::TRANSFORM_KEY) {
            hasTransform = true;
        }
        if (hasPrimitive && hasTransform) {
            return true;
        }
    }
    return false;
}

void JamJar::Standard::_2D::PrimitiveSystem::OnMessage(JamJar::Message *message) {
    MapSystem::OnMessage(message);
    switch (message->m_type) {
    case JamJar::Game::MESSAGE_PRE_RENDER: {
        auto *renderMessage = static_cast<JamJar::MessagePayload<float> *>(message);
        this->preRender(renderMessage->m_payload);
        break;
    }
    }
}

void JamJar::Standard::_2D::PrimitiveSystem::preRender(float alpha) {
    std::vector<Renderable> renderables;
    for (const auto &entityPair : this->m_entities) {
        auto entity = entityPair.second;
        auto *transform = static_cast<JamJar::Standard::_2D::Transform *>(
            entity.Get(JamJar::Standard::_2D::Transform::TRANSFORM_KEY));
        auto *primitive = static_cast<JamJar::Standard::_2D::Primitive *>(
            entity.Get(JamJar::Standard::_2D::Primitive::PRIMITIVE_KEY));

        renderables.push_back({primitive->m_points, transform->InterpolatedMatrix4D(alpha)});
    }
    auto msg = std::make_unique<JamJar::MessagePayload<std::vector<Renderable>>>(
        JamJar::Standard::_2D::RenderSystem::MESSAGE_LOAD_RENDERABLES, renderables);
    this->m_messageBus->Publish(std::move(msg));
}
