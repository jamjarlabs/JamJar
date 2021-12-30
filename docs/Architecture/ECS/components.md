# Components

A component is some data that is attached to an entity. A component in isolation does not do anything, it **must** be
attached to an entity to have any effect.

## Adding Components to an Entity

Components can be added to an entity by using the `Add` method on an `Entity`.

```c++
#include "geometry/vector_2d.hpp"
#include "entity/entity.hpp"
#include "standard/2d/transform/transform.hpp"

using JamJar::Vector2D;
using JamJar::Entity;
using JamJar::Standard::_2D::Transform;

...

auto player = new Entity(messageBus);
player->Add(new Transform(Vector2D(0, 20)));
```

The above code will add a `Transform` component to the `player` entity, setting its position to `x: 0, y: 20`.

The component is added as a raw pointer, but behind the scenes this will be picked up by the engine and the memory
managed - do not delete component pointers, let JamJar handle that.

### System Entities

If interacting with entities inside a `System` the entities are normally wrapped in a `SystemEntity` which also contains
an `Add` method which can be used in exactly the same way as on an `Entity`.

```c++
#include "geometry/vector_2d.hpp"
#include "entity/entity.hpp"
#include "standard/2d/transform/transform.hpp"

using JamJar::Vector2D;
using JamJar::Entity;
using JamJar::Message;
using JamJar::Standard::_2D::Transform;

...

void SomeSystem::OnMessage(Message *message) {
    MapSystem::OnMessage(message);
    switch (message->type) {
    case SOME_MESSAGE: {
        auto player = this->entities.begin()->second;
        player->Add(new Transform(Vector2D(0, 20)));
    }
    }
}
```

The above code will add a `Transform` to an existing `SystemEntity` when the `SOME_MESSAGE` message is received.

## Interacting with Components

Most interactions with Components should occur inside a `System` and should be done through the `SystemEntity` - the
`Get` method on the `SystemEntity` is a template function that takes a `Component` type and returns any `Component`
associated with the entity. This makes use of the `Component` `KEY` static constant so it is important any custom
component provides this.

```c++
#include "geometry/vector_2d.hpp"
#include "entity/entity.hpp"
#include "standard/2d/transform/transform.hpp"

using JamJar::Vector2D;
using JamJar::Entity;
using JamJar::Message;
using JamJar::Standard::_2D::Transform;

...

void SomeSystem::OnMessage(Message *message) {
    MapSystem::OnMessage(message);
    switch (message->type) {
    case SOME_MESSAGE: {
        auto player = this->entities.begin()->second;
        auto transform = entity.Get<Transform>();
        transform.position.x = 5;
    }
    }
}
```

## Creating a Component

You can create your own component by extending the `JamJar::Component` base class, every component must also provide
a unique `KEY` static constant which is used to distinguish between components.

```c++
#ifndef YOUR_COMPONENT_HPP
#define YOUR_COMPONENT_HPP

#include "component/component.hpp"

class YourComponent : public JamJar::Component {
  public:
    constexpr static uint32_t KEY = JamJar::hash("your_component");
    explicit YourComponent();
};

#endif
```

The code above is a header that describes a custom component, note the `KEY` static constant, this is required and
should be a unique hash value.

```c++
#include "your_component.hpp"

YourComponent::YourComponent() : Component(YourComponent::KEY) {}
```

The code above is the implementation of the custom component.

## Identifying a Component

A component is identified by an unsigned 32-bit integer, which should be largely unique. To help create unique
identifiers a compile time hashing function is used, for example:

```c++
constexpr static uint32_t KEY = JamJar::hash("jamjar_transform");
```

This tries to create a pseudo unique identifier for the Transform component, however hash collisions could occur - this
can be fixed by adjusting the string that is hashed slightly to produce a different result which will hopefully not
collide.

## Data

A component can hold any data, it could contain the position, scale, and rotation of an entity (Transform component),
it could hold the number of lives a player has, it could hold sprite information for drawing an entity with details
about color, texture, and shape.

The component is the single source of truth for this data, and multiple systems may read and write to the same
component - this is a strengh of ECS, shared data can be read and modified by multiple systems.

## Memory

When adding a new component using one of the `Add` methods on the `Entity` or `SystemEntity` these methods take a raw
pointer to the component, then converts them to a unique pointer which JamJar manages. This means that the developer
does not need to manage Component memory.

## Where are Components Stored?

See the [ComponentManager] and [EntityManager] for details.

[ComponentManager]:./internals/component_manager.md
[EntityManager]:./internals/entity_manager.md
