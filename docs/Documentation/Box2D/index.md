# Box2D

JamJar includes the [Box2D physics engine](https://github.com/erincatto/box2d) to handle all of the Physics
calculations. There is a wrapper around the Box2D APIs to make it integrate better with the JamJar architecture
using entities, components, and systems.

The `Box2DPhysicsSystem` manages Box2D and acts as a bridge between Box2D and JamJar.

## Setting up the Box2D Physics System

To set up a `Box2DPhysicsSystem` you need to provide a message bus alongside a `Vector2D` representing the gravity
to apply to Box2D objects.

```c++
#include "standard/2d/box2d/box2d_physics_system.hpp"
#include "geometry/vector_2d.hpp"
...
new JamJar::Standard::_2D::Box2DPhysicsSystem(messageBus, JamJar::Vector2D(0, -10.0f));
```

This sets up the `Box2DPhysicsSystem` with the world gravity set to `x: 0, y: -10.0f` (gravity downwards).
