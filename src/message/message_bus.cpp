#include "message/message_bus.hpp"
#include "message/listener.hpp"
#include "message/message.hpp"

JamJar::MessageBus::MessageBus() {
    this->m_subscribers = std::map<uint32_t, std::vector<Listener *>>();
    this->m_messages = std::queue<std::unique_ptr<Message>>();
}

void JamJar::MessageBus::Dispatch() {
    while (!this->m_messages.empty()) {
        m_messages.front();
        std::unique_ptr<Message> message = std::move(m_messages.front());
        m_messages.pop();

        if (this->m_subscribers.count(message->type) == 0) {
            continue;
        }

        std::vector<Listener *> subscribers = this->m_subscribers[message->type];

        for (auto *subscriber : subscribers) {
            subscriber->OnMessage(message.get());
        }
    }
}

void JamJar::MessageBus::Publish(std::unique_ptr<Message> message) { this->m_messages.push(std::move(message)); }

void JamJar::MessageBus::Subscribe(Listener *subscriber, uint32_t type) {
    if (this->m_subscribers.count(type) == 0) {
        this->m_subscribers[type] = std::vector<Listener *>();
    }
    this->m_subscribers[type].push_back(subscriber);
}
