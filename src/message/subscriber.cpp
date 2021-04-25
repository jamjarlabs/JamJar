#include "message/subscriber.hpp"
#include <cstdint>

uint32_t JamJar::Subscriber::SubscriberID = 0;

JamJar::Subscriber::Subscriber() : m_subscriberID(JamJar::Subscriber::SubscriberID++) {}
