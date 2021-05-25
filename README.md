# JamJarNative

JamJarNative is a 2D, C++ game engine that primarily targets running in the browser through web assembly.

## Features

- Entity-Component-System design.
- Messaging through a centralised message bus.
- Modular design, choose to remove parts of the engine and replace parts.
- Customisable render pipeline.
- Rendering to a WebGL2 canvas.
- Simple motion based physics.
- Interpolation between frames.
- Orthographic camera projection.
- Sprites.
- Image loading from filesystem (virtual or otherwise).

## Getting Started

// TODO: add this guide

## Examples

See the [examples directory for working code examples](./examples).

## Dependencies

- [STB](https://github.com/nothings/stb) - MIT/Public domain

**Dev Dependencies**

- [GoogleTest](https://github.com/google/googletest) - BSD-3
- [Emscripten](https://github.com/emscripten-core/emscripten) - MIT/UIUC
- [llvm/clang/clang-tidy/clang-format](https://llvm.org/) - UIUC/Apache-2.0 with LLVM Exceptions
- [CMake](https://cmake.org/) - BSD-3

## Contributing

See the [contributing guide](./CONTRIBUTING.md).
