# Physics Resizing

This is an example that demonstrates how to resize an entity that uses a `Box2DBody`.

## Overview

This example uses a custom `ResizingSystem` paired with a `Resizable` component, this specifies a max and minimum scale
to scale to. If the scale goes above the max, it will start decreasing, until it hits the minimum and then it will
start increasing again.

The key part of this example is that it uses the `Box2DBody.SetScale(scale)` function to adjust the scale, rather
than modifiying the `Transform`.

## Dependencies

See the dependencies for [JamJar here](../../README.md#Dependencies).

## Running this example

1. Clone down the JamJar project:

```bash
git clone git@github.com:jamjarlabs/JamJar.git
```

2. Navigate the cloned directory:

```bash
cd JamJar
```

3. Use `cmake` to generate the build system and pull down any dependencies:

```bash
emcmake cmake -D CMAKE_EXPORT_COMPILE_COMMANDS=ON . -B build
```

4. Navigate to the generated example build directory:

```bash
cd build/examples/PhysicsResizing
```

5. Build the project:

```bash
emmake make
```

6. If you want to view the example, you can now access the generated HTML, if you have Python 3 installed a handy way
to do this is to run:

```bash
python -m http.server
```

Then you should be able to access the game at <http://127.0.0.1:8000/>.
