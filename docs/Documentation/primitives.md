# Primitives

Primitives are renderable components which give greater (low level) control over the rendering process. Primitives
can be used to directly render points, lines, and polygons.

The `PrimitiveSystem` allows rendering `Primitive` components.

## Setting up the Primitive System

To set up a `PrimitiveSystem`, you simply need to provide a message bus.

```c++
#include "standard/2d/primitive/primitive_system.hpp"
...
new JamJar::Standard::_2D::PrimitiveSystem(messageBus);
```

## Using a Primitive

To use a `Primitive` it needs to be added to an entity, and it requires a `Transform` component alongside it to work.

```c++
auto triangle = new JamJar::Entity(messageBus);
triangle->Add(new JamJar::Standard::_2D::Transform(JamJar::Vector2D(0, 0), JamJar::Vector2D(30, 30)));
triangle->Add(new JamJar::Standard::_2D::Primitive(
    JamJar::Polygon({0, 0.5, 0.5, -0.5, -0.5, -0.5, 0, 0.5}), JamJar::Material(JamJar::Color(0, 1, 0, 1))));
```

This will create a simple triangle, defined by the `Polygon` provided to the `Primitive`. The visual element of the
`Primitive` is defined by the `Material` provided, in the example above it is just a green colour applied.

A `Primitive` can be configured with the following definitions:

- `Polygon` - required, the shape of the primitive.
- `Material` - required, the visual material to apply to the primitive, e.g. a texture or colour.
- `DrawMode` - optional, default is `LINE_STRIP` (see the [available draw modes here](./draw_modes.md)).

## Peer Dependencies

The `PrimitiveSystem` will only work in conjunction with a 2D rendering system, such as the `WebGL2System` for
rendering in browser to a HTML5 canvas. The `PrimitiveSystem` is only a *pre-rendering system*, it does not actually
draw the sprites, instead it prepares the sprites and converts them into a format that a *rendering system* can parse
and use to draw with.
