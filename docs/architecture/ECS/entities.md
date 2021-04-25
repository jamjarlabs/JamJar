# Entities

An entity is a singular *thing*.

## What Actually is an Entity?

An entity is basically just a unique ID that allows a thing to be identified, the entity itself does not store any
components (data), instead it is just an index that the components can be grouped with. The entity includes a couple of
helper functions to allow components to be managed (adding and removing), and to allow the entity  to be destroyed - but
keep in mind these are only helpers, the component data is not actually stored in the entity, it is stored elsewhere in
the engine (see the [ComponentManager] and [EntityManager] for details about this).

## Where are Entities Stored?

See the [EntityManager] for details.

## Granularity

The granularity of this will depend on the design and choices for a game - a player could be a single entity, or it
could be multiple entities, with one for the player's head, another for the player's torso etc. This is simply a design
choice, and working out the best approach is left to the game developer.

[ComponentManager]:./internals/component_manager.md
[EntityManager]:./internals/entity_manager.md
