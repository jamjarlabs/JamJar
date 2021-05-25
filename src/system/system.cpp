#include "system/system.hpp"
#include "entity/entity.hpp"
#include "message/message_payload.hpp"

JamJar::System::System(MessageBus *messageBus) : messageBus(messageBus) {
    this->messageBus->Subscribe(this, JamJar::System::MESSAGE_UPDATE);
}

void JamJar::System::OnMessage(JamJar::Message *message) {
    switch (message->type) {
    case JamJar::System::MESSAGE_UPDATE: {
        auto *updateMessage = static_cast<JamJar::MessagePayload<float> *>(message);
        this->update(updateMessage->payload);
        break;
    }
    }
}
