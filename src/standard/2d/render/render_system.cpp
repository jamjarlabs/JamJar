#include "standard/2d/render/render_system.hpp"
#include "game.hpp"
#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "message/message_payload.hpp"
#include "standard/2d/render/renderable.hpp"

JamJar::Standard::_2D::RenderSystem::RenderSystem(MessageBus *messageBus, Evaluator evaluator)
    : VectorSystem(messageBus, std::move(evaluator)) {
    this->messageBus->Subscribe(this, JamJar::Game::MESSAGE_RENDER);
    this->messageBus->Subscribe(this, JamJar::Standard::_2D::RenderSystem::MESSAGE_LOAD_RENDERABLES);
}

void JamJar::Standard::_2D::RenderSystem::OnMessage(JamJar::Message *message) {
    VectorSystem::OnMessage(message);
    switch (message->type) {
    case JamJar::Standard::_2D::RenderSystem::MESSAGE_LOAD_RENDERABLES: {
        auto *loadMessage = static_cast<JamJar::MessagePayload<std::vector<Renderable>> *>(message);
        this->renderables.insert(this->renderables.end(), loadMessage->payload.begin(), loadMessage->payload.end());
        break;
    }
    case JamJar::Game::MESSAGE_RENDER: {
        auto *renderMessage = static_cast<JamJar::MessagePayload<float> *>(message);
        this->render(renderMessage->payload);
        break;
    }
    }
}
