#include "system/stateful_system.hpp"
#include "entity/entity_manager.hpp"
#include "message/message_payload.hpp"

JamJar::StatefulSystem::StatefulSystem(MessageBus *messageBus) : System(messageBus) {
    this->m_messageBus->Subscribe(this, JamJar::EntityManager::MESSAGE_REGISTER);
    this->m_messageBus->Subscribe(this, JamJar::EntityManager::MESSAGE_DEREGISTER);
}

void JamJar::StatefulSystem::OnMessage(JamJar::Message *message) {
    System::OnMessage(message);
    switch (message->m_type) {
    case JamJar::EntityManager::MESSAGE_REGISTER: {
        auto *registerMessage =
            static_cast<JamJar::MessagePayload<std::unique_ptr<JamJar::RegisterEntityPayloadPair>> *>(message);
        this->registerEntity(registerMessage->m_payload->first, registerMessage->m_payload->second);
        break;
    }
    case JamJar::EntityManager::MESSAGE_DEREGISTER: {
        auto *registerMessage = static_cast<JamJar::MessagePayload<unsigned int> *>(message);
        this->removeEntity(registerMessage->m_payload);
        break;
    }
    }
}
