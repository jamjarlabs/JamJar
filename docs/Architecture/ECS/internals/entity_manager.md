# Entity Manager

The entity manager is the central store of all entities, managing the lifecycle and memory of entities, while also
keeping track of component managers which manage all components.

## Entity Lifecycle

An entity is managed by the entity manager, including its lifecycle which is important for managing an entity's memory:

1. An entity is created, it is sent to the entity manager to track.
2. A component is added to the entity, the entity manager handles redirecting the component to the correct component
manager.
3. The entity is destroyed, any components attached to the entity are also destroyed with the entity manager informing
the required component managers, then the entity is deregistered from any stateful systems and erased in the entity
manager's internal store.

## Component Changes

When there is any kind of component change on an entity, the entity manager will register this change by sending out
a message with the entity and its full component selection. This is listened to by stateful systems (see the [Stateful
Systems] page for more details) which will then run some evaluation logic to determine if the entity should still be
tracked by the stateful system.

Components are not directly stored in the entity manager, instead component managers track different component types
and manage their lifecycles/memory.

See the [Component Manager] page for more details about component managers.

### Adding a Component

When a component is added the entity manager will first check if there is a component manager assigned for the
component type, if not it will initialize one, before adding the component to the component manager and re-registering
the entity with any stateful systems.

### Removing a Component

When a component is removed the entity manager will remove the component from any component managers (destroying the
component and its data) before re-registering the entity with any stateful systems.

[Stateful Systems]:../stateful_systems.md
[Component Manager]:./component_manager.md
