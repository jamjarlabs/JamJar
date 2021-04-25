# Subscribers

A subscriber is a part of the engine that can receive messages, and can be used with the message bus to subscribe
to specific message types. A subscriber can be set up by extending the Subscriber class.

A subscriber is just made up of a unique ID (an unsigned 32-bit integer) and a public `OnMessage` method which allows
the message bus to send messages to the subscriber.

See the [Message Bus] page for more details of how the message bus uses subscribers.

[Message Bus]:./message_bus.md
