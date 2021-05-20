#include "message/listener.hpp"
#include <cstdint>

uint32_t JamJar::Listener::ID = 0;

JamJar::Listener::Listener() : m_listenerID(JamJar::Listener::ID++) {}
