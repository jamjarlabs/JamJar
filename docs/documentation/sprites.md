# Sprites

Sprites are simple renderable components, allowing shapes to be rendered with materials applied to them to specify
texture, color and shaders.

The `SpriteSystem` allows rendering `Sprite` components.

## Setting up the Sprite System

To set up a `SpriteSystem`, you simply need to provide a message bus.

```c++
new JamJar::Standard::_2D::SpriteSystem(messageBus);
```

## Peer Dependencies

The `SpriteSystem` will only work in conjunction with a 2D rendering system, such as the `WebGL2System` for rendering in
browser to a HTML5 canvas. The `SpriteSystem` is only a *pre-rendering system*, it does not actually draw the sprites,
instead it prepares the sprites and converts them into a format that a *rendering system* can parse and use to
draw with.
