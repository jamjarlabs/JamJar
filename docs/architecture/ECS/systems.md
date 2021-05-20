# Systems

A system is simply a piece of logic that runs.

## Logic

The description above is pretty vague, but this is because a system is not constrained in what it can and can't do,
it can operate on entities and components (for example the MotionSystem, which updates position and motion data every
update), or it can operate without any entities and components, sending and receiving messages (for example a system
that stops the game when the player is destroyed). These systems are Stateless Systems (does not track entities or
components) and Stateful Systems (tracks entities and components).

## Messages

All systems are [Listeners], meaning that they can receive messages, alongside this all systems store a reference
to a [Message Bus] which allows them to send messages.

Systems can override the `OnMessage` method to handle messages, the base system includes basic handling already which
listens for an update message, and calls the protected `update` which can be overridden to add logic that runs
on every update.

> Be aware that if a system does override the `OnMessage` method, the base `OnMessage` method should be called if the
> system still needs to handle updates.

## Stateless Systems

Stateless Systems (just referred to as Systems as they are the base class) do not track any entities or components,
they simply send and receive messages.

## Stateful Systems

See the [Stateful Systems] page for details.

[Listeners]:../messages/listeners.md
[Message Bus]:../messages/message_bus.md
[Stateful Systems]:./stateful_systems.md
