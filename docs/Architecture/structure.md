# Structure

The game engine is structured into two distinct parts. The *core* engine handles and defines the [Entity
Component System] design, the [Messages] pattern, and shared resources such as geometry. The *standard* library
provides useful common components and systems, such as rendering to a WebGL2 canvas, an interpolation system, a simple
physics movement system, and more.

## Headers

You can see this structure in the headers for the library, it is structured like this:

- `game.hpp`
- `message/message.hpp`
- `geometry/vector_2d.hpp`
- `standard/2d/sprite/sprite.hpp`

As you can see anything that is in the `standard/` directory/path is in the standard library.

[Entity Component System]:./ECS/index.md
[Messages]:./Messages/index.md
