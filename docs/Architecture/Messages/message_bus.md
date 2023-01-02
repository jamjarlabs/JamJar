# Message Bus

The message bus is the core of the engine, it is used to manage sending messages between different parts of the
engine/game.

## Adding a Message to the Message Bus

A message can be added to the message bus by using the `Publish` method, this will not immediately dispatch the message,
instead it will be stored until a `Dispatch` occurs.

Messages added to the message bus should be added as a unique pointer
([`std::unique_ptr`](http://www.cplusplus.com/reference/memory/unique_ptr/)) - this is because ownership of the memory
that a message is sent with should be held by the message bus, helping to prevent memory leaks and memory management
confusion. The message bus will take ownership of the pointer and will clean it up after it has been sent out.

An example of adding a message to the message bus looks like this:

```c++
this->m_messageBus->Publish(new JamJar::MessagePayload<float>(
    JamJar::System::MESSAGE_UPDATE, float(TIME_STEP) / MICROSECOND_TO_SECOND_CONVERSION));
```

This dispatches a new update message, with the delta time as a payload.

The message is given as a pointer, but once it's published its memory will be managed by the message bus, so you should
not re-use messages, each time you publish create a new message. The message will be disposed of once it is no longer
needed.

Behind the scenes the message is wrapped in a `unique_ptr` - you can instead directly pass in a `unique_ptr` message
to the `Publish` method if you prefer:

```c++
this->m_messageBus->Publish(std::make_unique<JamJar::MessagePayload<float>>(
    JamJar::System::MESSAGE_UPDATE, float(TIME_STEP) / MICROSECOND_TO_SECOND_CONVERSION));
```

## Dispatching Messages

Every message in the message bus is *dispatched* - sent out to any subscribing part of the engine - when the `Dispatch`
method of the message bus is called. This will iterate through every message, in the order that they are published
to the message bus, and will fetch all subscribers to the specific message type, each subscriber will be sent the
message by calling their `OnMessage` method.

The `Dispatch` function will keep calling until it is completely empty, meaning that you should be careful to not
introduce any circular dependencies of messages, as this could cause an infinite loop. An example of a circular
dependency that would cause this is:

1. A sends out message X, B is a subscriber to X, so the message X is sent to B.
2. Once B receives the message, it immediately sends out message Y.
3. A is a subscriber to Y, so the message Y is sent to A.
4. Once A receives the message, it immediately sends out message X.
5. This loops on infinitely.

### When Does Dispatching Happen?

Message dispatching should probably not be triggered by game code, instead it should be left to the engine to decide
when to call it. The engine will dispatch messages in these three instances (all in the Game class):

1. After every update message is added, in the main update loop.
2. After the pre-rendering stage of the render pipeline.
3. After the rendering and post-rendering stage of the render pipeline.

## Subscribing to Message Types

A part of the game/engine can *subscribe* to certain message types in the message bus by using the `Subscribe` method,
which takes a listener and a message type to filter for. This means that any message that has the type subscribed
to will be sent on to the listener.

An example of a listener subscribing to a message is the System subscribing to update messages:

```c++
this->m_messageBus->Subscribe(this, JamJar::System::MESSAGE_UPDATE);
```

See the [Listeners] page for more details about listeners.

[Listeners]:./listeners.md
