# SDL2

JamJar includes the [SDL2 library](https://github.com/libsdl-org/SDL) to handle user input, window management, and
fullscreen functionality. There is a wrapper around the SDL2 APIs to make it integrate better with the JamJar
architecture using entities, components, and systems.

The `SDL2InputSystem` handles converting SDL2 user input into a JamJar ECS friendly format through messages.

The `WindowSystem` makes use of SDL2 to help when calculating screen sizes for fullscreen.

## Setting up the SDL2 Input System

To set up a `SDL2InputSystem` you only need to provide a message bus:

```c++
#include "standard/sdl2_input/sdl2_input_system.hpp"
...
new JamJar::Standard::SDL2InputSystem(messageBus);
```
