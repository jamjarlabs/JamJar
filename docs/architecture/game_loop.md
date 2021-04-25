# Game Loop

The game loop in the JamJarNative engine is built following the article ['Fix Your Timestep!' by Glenn
Fiedler](https://gafferongames.com/post/fix_your_timestep/). The article sets out nicely a progression in game loops
before concluding with a recommended game loop. To understand the game loop better, read the article, but the key
points of the approach JamJarNative uses are laid out here:

- Rendering can occur as fast as possible.
- Physics/Game calculations are uncoupled from this render/loop rate.
- Physics/Game updates can catch up if running behind.
- Physics/Game updates will slow down if ahead.

The basic algorithm that JamJarNative uses is:

```
timestep = 1/60
currentTime = 0
accumulator = 0
running = true

while(running)
    timestamp = now()

    frameTime = timestamp - currentTime
    if frameTime > 0.25
        frameTime = 0.25

    currentTime = timestamp
    accumulator += frameTime

    while accumulator >= timestep
        update(timestep)
        accumulator -= timestep

    alpha = accumulator / timestep

    render(alpha)
```

## Timestep

The timestep of JamJarNative is for an update to occur every 1/60th of a second (60 FPS). When running with emscripten
it will use the [requestAnimationFrame] browser function, which is limited to 60 FPS.

### Interpolation

Because of the game loop used JamJarNative must interpolate as part of the rendering process, this means taking the last
rendered position of an object and the current position of the object and rendering at the halfway between these
two positions.

See the Transform component and InterpolationSystem to see how this previous position is tracked and updated.

[requestAnimationFrame]:https://developer.mozilla.org/en-US/docs/Web/API/window/requestAnimationFrame
