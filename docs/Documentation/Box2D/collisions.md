# Collisions

JamJar provides a way to natively detect and respond to collisions detected by Box2D by allowing systems to listen
out and parse collisions from messages.

## Listening For a Collision

To listen for a collision, a system must first subscribe to the message event.

```c++
#include "standard/2d/box2d/box2d_physics_system.hpp"

...

YourCustomSystem::YourCustomSystem(JamJar::MessageBus *messageBus): JamJar::System(messageBus) {
    this->messageBus->Subscribe(this, JamJar::Standard::_2D::Box2DPhysicsSystem::MESSAGE_COLLISION_ENTER);
    this->messageBus->Subscribe(this, JamJar::Standard::_2D::Box2DPhysicsSystem::MESSAGE_COLLISION_EXIT);
}
```

This subscribes the `YourCustomSystem` system to the messages sent when a collision is entered and exited.

## Parsing a Collision

After subscribing a system to a collision event it will then recieve any collision messages through the `OnMessage`
method. Messages should be filtered and responded to based on the message type, so it's good practice to use a switch
statement here to differentiate between message types.

After getting a collision message it will always have a payload attached to the message, so a static cast can be used
to retrieve this collision data in the form of a `Box2DCollision`.

```c++
#include "standard/2d/box2d/box2d_body.hpp"
#include "standard/2d/box2d/box2d_physics_system.hpp"
#include <iostream>

...

void YourCustomSystem::OnMessage(JamJar::Message *message) {
    System::OnMessage(message);
    switch (message->type) {
    case JamJar::Standard::_2D::Box2DPhysicsSystem::MESSAGE_COLLISION_ENTER: {
        auto *collisionMessage = static_cast<JamJar::MessagePayload<JamJar::Standard::_2D::Box2DCollision> *>(message);
        auto collision = collisionMessage->payload;
        std::cout << "Entity A in the collision enter is: " << collision.aID << std::endl;
        std::cout << "Entity B in the collision enter is: " << collision.bID << std::endl;
        break;
    }
    switch (message->type) {
    case JamJar::Standard::_2D::Box2DPhysicsSystem::MESSAGE_COLLISION_EXIT: {
        auto *collisionMessage = static_cast<JamJar::MessagePayload<JamJar::Standard::_2D::Box2DCollision> *>(message);
        auto collision = collisionMessage->payload;
        std::cout << "Entity A in the collision exit is: " << collision.aID << std::endl;
        std::cout << "Entity B in the collision exit is: " << collision.bID << std::endl;
        break;
    }
    }
}
```

This handles the messages when they are published and dispatched, with the `YourCustomSystem` system parsing out the
entity IDs from each collision and printing them.

### Collision Data

The data in a collision is represented by the `Box2DCollision` object, which holds the following pieces of information:

- The IDs of the entities involved in the collision.
- A Box2D `b2Contact` object which describes the collision in depth, see the [Box2D docs for full
details](https://box2d.org/documentation/classb2_contact.html).
- The type of the collision, defined as an enum `Box2DCollisionType`, this describes the collision event, e.g. collision
enter or exit.
