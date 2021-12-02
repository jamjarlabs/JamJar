[![Build](https://github.com/jamjarlabs/JamJar/workflows/build/badge.svg)](https://github.com/jamjarlabs/JamJar/actions)
[![Documentation Status](https://readthedocs.org/projects/jamjar/badge/?version=latest)](https://engine.jamjarlabs.com/en/latest/?badge=latest)
[![MIT License](https://img.shields.io/:license-mit-blue.svg)](https://choosealicense.com/licenses/mit/)

# JamJar

JamJar is a 2D, C++ game engine that primarily targets running in the browser through web assembly.

## Features

- Entity-Component-System design.
- Messaging through a centralised message bus.
- Modular design, choose to remove parts of the engine and replace parts.
- Customisable render pipeline.
- Rendering to a WebGL2 canvas.
- Interpolation between frames.
- Orthographic camera projection.
- Sprites.
- Image loading from filesystem (virtual or otherwise).
- Input using SDL2 events (keyboard and mouse).
- Box2D based Physics system.
- Collision detection using Box2D.
- Window management system.

## Getting Started

// TODO: add this guide

## Documentation

See the [documentation for guides and architecture details](https://engine.jamjarlabs.com/en/latest/).

## Examples

See the [examples directory for working code examples](./examples).

## Dependencies

- [STB](https://github.com/nothings/stb) - MIT/Public domain
- [SDL2](https://github.com/libsdl-org/SDL) - ZLib
- [Box2D](https://github.com/erincatto/box2d) - MIT

**Dev Dependencies**

- [GoogleTest](https://github.com/google/googletest) - BSD-3
- [Emscripten](https://github.com/emscripten-core/emscripten) - MIT/UIUC
- [llvm/clang/clang-tidy/clang-format](https://llvm.org/) - UIUC/Apache-2.0 with LLVM Exceptions
- [CMake](https://cmake.org/) - BSD-3

## Contributing

See the [contributing guide](./CONTRIBUTING.md).
