# Listener

A listener is a part of the engine that can receive messages, and can be used with the message bus to subscribe
to specific message types. A listener can be set up by extending the Listener class.

A listener is just made up of a unique ID (an unsigned 32-bit integer) and a public `OnMessage` method which allows
messages to be sent to and processed by the listener

See the [Message Bus] page for more details of how the message bus uses listeners to subscribe to message types.

[Message Bus]:./message_bus.md
