#include "system/system.hpp"
#include "entity/entity.hpp"
#include "message/message_payload.hpp"

JamJar::System::System(MessageBus *messageBus) : m_messageBus(messageBus) {
    this->m_messageBus->Subscribe(this, JamJar::System::MESSAGE_UPDATE);
}

void JamJar::System::OnMessage(JamJar::Message *message) {
    switch (message->m_type) {
    case JamJar::System::MESSAGE_UPDATE: {
        auto *updateMessage = static_cast<JamJar::MessagePayload<float> *>(message);
        this->update(updateMessage->m_payload);
        break;
    }
    }
}
