# Components

A component is some data that is attached to an entity. A component in isolation does not do anything, it **must** be
attached to an entity to have any effect.

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
