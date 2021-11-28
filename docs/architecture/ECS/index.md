# ECS

JamJarNative uses an *Entity Component System* (ECS) design at its core, this pattern drives much of the
architecture of the engine.

## Overview

ECS design is splitting up the engine and any games that use it into [Entities], [Components], and [Systems]. These
three components should be able to cover any functionality that a game needs.

An entity is a single *thing* in the game, for example a player, a bullet, or a camera.

A component is a piece of data that is attached to an entity, for example position (Transform), an image (Sprite), or
the player's number of lives.

A system is some logic that can operate on entities and components to interpret them and modify them, for example
the SpriteSystem which takes Sprite descriptions and converts them into a format that the render pipeline can use to
draw shapes.

## Benefits

- Decoupling of game logic, helps keeps direct dependencies to a minimum.
- Consistent method of structuring the game and engine.
- Things in the game are grouped together using composition rather than inheritance, meaning traits can be easily
added and removed from things, and shared between things.

## Limitations

- Can be difficult to understand when coming from an Object Oriented Programming approach.
- Sometimes can result in a lot of 'boilerplate' type code for some simple functionality.
- Concurrency is hard.

[Entities]:./entities.md
[Components]:./components.md
[Systems]:./systems.md
