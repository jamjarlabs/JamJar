# Messages

JamJarNative extensively uses *messages* to send information between different parts of the engine. This allows for
different parts of the system to be decoupled and not directly depend on eachother. The pattern that messages use
is broadcast and listen - a message is broadcast, and parts of the engine can *subscribe* to message types to listen
for. This means a single message may be sent to multiple listeners, or *subscribers*.

## Message Types

A message can be identified by a supplied type, which allows subscription to a certain message type, and conditional
behaviour based on the type. The type is an unsigned 32-bit integer, which should be largely unique. To help create
unique message types a compile time hashing function is used, for example:

```c++
constexpr static uint32_t MESSAGE_UPDATE = hash("jamjar_update");
```

## Messages without Payloads

Messages do not have to contain any other data other than the message type (using the base Message class), which may be
useful for messages which are just triggers/events - for example a message that is sent when the player's score is
incremented.

## Messages with Payloads

A message can contain an optional payload (using the MessagePayload class) which accepts a generic payload that can
contain any data - for example the the delta time is included with all update messages as a payload.

## Broadcasting Messages

See the [Message Bus] page for details.

## Receiving Messages

See the [Subscribers] page for details.

[Message Bus]:./message_bus.md
[Subscribers]:./subscribers.md
