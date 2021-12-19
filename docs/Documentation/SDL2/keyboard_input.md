# Keyboard Input

The `SDL2InputSystem` broadcasts keyboard events, which can be listened to and parsed to handle keyboard input.

## Subscribing to Key Events

There are two keyboard events that can be subscribed to, `MESSAGE_KEY_DOWN_EVENT` and `MESSAGE_KEY_UP_EVENT`.

```c++
#include "standard/sdl2_input/sdl2_input_system.hpp"
...
YourSystem::YourSystem(JamJar::MessageBus *messageBus) : JamJar::System(messageBus) {
    this->messageBus->Subscribe(this, JamJar::Standard::SDL2InputSystem::MESSAGE_KEY_DOWN_EVENT);
    this->messageBus->Subscribe(this, JamJar::Standard::SDL2InputSystem::MESSAGE_KEY_UP_EVENT);
}
```

## Parsing Key Events

Each key event message has a payload which is of type `SDL2KeyEvent` which contains some useful quick access
information, alongside the raw `SDL_Event` that gives full advanced key event information.

The quick access information in the `SDL2KeyEvent` is:

- Key name as a character array.
- The key event type, expressed as an `SDL2KeyEventType` enum.
- If the key event is a repeat (holding down a key will generate multiple key down events, the duplicates are marked as
repeat, this is useful for handling user text input).

See the SDL2 documentation for a full reference of the raw [SDL_Event](https://wiki.libsdl.org/SDL_Event).

```c++
#include "standard/sdl2_input/sdl2_input_system.hpp"
...
void YourSystem::OnMessage(JamJar::Message *message) {
    System::OnMessage(message);
    switch (message->type) {
    case JamJar::Standard::SDL2InputSystem::MESSAGE_KEY_DOWN_EVENT: {
        auto *eventMessage = static_cast<JamJar::MessagePayload<JamJar::Standard::SDL2KeyEvent> *>(message);
        auto event = eventMessage->payload;
        if (event.repeat) {
            // Ignore repeat keypresses
            break;
        }
        std::cout << "listener got key down: " << event.key << std::endl;
        break;
    }
    case JamJar::Standard::SDL2InputSystem::MESSAGE_KEY_UP_EVENT: {
        auto *eventMessage = static_cast<JamJar::MessagePayload<JamJar::Standard::SDL2KeyEvent> *>(message);
        auto event = eventMessage->payload;
        if (event.repeat) {
            break;
        }
        std::cout << "listener got key up: " << event.key << std::endl;
        break;
    }
    }
}
```
