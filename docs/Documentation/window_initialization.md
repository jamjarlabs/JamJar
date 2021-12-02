# Window Initialization

JamJar provides some utility functions to help initialize the game window, wrapping around the SDL2 API and providing
some sensible defaults to hide complexity.

```c++
#include "window.hpp"

int main(int argc, char *argv[]) {

    auto window = JamJar::GetWindow("Sprites");
    auto context = JamJar::GetCanvasContext();

    return 0;
}
```

This initializes the window to be called `Sprites`, then it gets and initializes the canvas context, allowing it to
be used to render.

These functions are simply wrappers around the `SDL_CreateWindow` and `emscripten_webgl_create_context` functions with
sensible defaults, allowing further configuration (e.g. passing window size, position).
