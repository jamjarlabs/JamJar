# Sprites

This is an example that demonstrates how to use Sprites to display simple textures.

The code is designed to be simple and easy to follow.

## Overview

This example displays two sprites using a single texture of a smiley face, it applies a green colour tint to one face
and a red colour tint to the other.

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
cd build/examples/Sprites
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
