#include "standard/render/render_system.hpp"
#include "game.hpp"
#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "message/message_payload.hpp"
#include "standard/render/renderable.hpp"
#include "system/evaluator.hpp"

JamJar::Standard::RenderSystem::RenderSystem(MessageBus *messageBus, Evaluator evaluator)
    : MapSystem(messageBus, std::move(evaluator)) {
    this->m_messageBus->Subscribe(this, JamJar::Game::MESSAGE_RENDER);
    this->m_messageBus->Subscribe(this, JamJar::Standard::RenderSystem::MESSAGE_LOAD_RENDERABLES);
}

void JamJar::Standard::RenderSystem::OnMessage(JamJar::Message *message) {
    MapSystem::OnMessage(message);
    switch (message->m_type) {
    case JamJar::Standard::RenderSystem::MESSAGE_LOAD_RENDERABLES: {
        auto *loadMessage = static_cast<JamJar::MessagePayload<std::vector<Renderable>> *>(message);
        this->renderables.insert(this->renderables.end(), loadMessage->m_payload.begin(), loadMessage->m_payload.end());
        break;
    }
    case JamJar::Game::MESSAGE_RENDER: {
        auto *renderMessage = static_cast<JamJar::MessagePayload<float> *>(message);
        this->render(renderMessage->m_payload);
        break;
    }
    }
}
