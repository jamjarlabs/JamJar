# Mouse Input

The `SDL2InputSystem` broadcasts mouse events, which can be listened to and parsed to handle mouse input.

## Subscribing to Mouse Events

There are four mouse events that can be subscribed to, `MESSAGE_MOUSE_BUTTON_UP_EVENT`,
`MESSAGE_MOUSE_BUTTON_DOWN_EVENT`, `MESSAGE_MOUSE_MOTION_EVENT`, and `MESSAGE_MOUSE_WHEEL_EVENT`.

```c++
#include "standard/sdl2_input/sdl2_input_system.hpp"
...
YourSystem::YourSystem(JamJar::MessageBus *messageBus) : JamJar::System(messageBus) {
    this->messageBus->Subscribe(this, JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_BUTTON_UP_EVENT);
    this->messageBus->Subscribe(this, JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_BUTTON_DOWN_EVENT);
    this->messageBus->Subscribe(this, JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_MOTION_EVENT);
    this->messageBus->Subscribe(this, JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_WHEEL_EVENT);
}
```

## Parsing Mouse Events

Each mouse event message has a payload which is of type `SDL2MouseEvent` which contains some useful quick access
information, alongside the raw `SDL_Event` that gives full advanced key event information.

The quick access information in the `SDL2MouseEvent` is:

- The mouse event type, expressed as an `SDL2MouseEventType` enum.
- An optional value containing the mouse button used, expressed as an optional `SDL2MouseButton` (this is not present
for mouse motion or mouse wheel events).
- The position of the mouse during the event expressed in terms of the canvas (top left canvas is `x:0, y:0`).

See the SDL2 documentation for a full reference of the raw [SDL_Event](https://wiki.libsdl.org/SDL_Event).

```c++
#include "standard/sdl2_input/sdl2_input_system.hpp"
...
void YourSystem::OnMessage(JamJar::Message *message) {
    System::OnMessage(message);
    switch (message->type) {
        case JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_BUTTON_DOWN_EVENT: {
        auto *eventMessage = static_cast<JamJar::MessagePayload<JamJar::Standard::SDL2MouseEvent> *>(message);
        auto event = eventMessage->payload;
        if (event.button == JamJar::Standard::SDL2MouseButton::LEFT) {
            std::cout << "listener got left mouse button down" << std::endl;
        } else if (event.button == JamJar::Standard::SDL2MouseButton::RIGHT) {
            std::cout << "listener got right mouse button down" << std::endl;
        } else {
            std::cout << "listener got a different mouse button down" << std::endl;
        }
        break;
    }
    case JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_BUTTON_UP_EVENT: {
        auto *eventMessage = static_cast<JamJar::MessagePayload<JamJar::Standard::SDL2MouseEvent> *>(message);
        auto event = eventMessage->payload;
        if (event.button == JamJar::Standard::SDL2MouseButton::LEFT) {
            std::cout << "listener got left mouse button up" << std::endl;
        } else if (event.button == JamJar::Standard::SDL2MouseButton::RIGHT) {
            std::cout << "listener got right mouse button up" << std::endl;
        } else {
            std::cout << "listener got a different mouse button up" << std::endl;
        }
        break;
    }
    case JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_WHEEL_EVENT: {
        auto *eventMessage = static_cast<JamJar::MessagePayload<JamJar::Standard::SDL2MouseEvent> *>(message);
        auto event = eventMessage->payload;
        if (event.event.wheel.y > 0) {
            std::cout << "mouse wheel up" << std::endl;
        } else if (event.event.wheel.y < 0) {
            std::cout << "mouse wheel down" << std::endl;
        }
        break;
    }
    }
}
```

### Getting the World Position of a Mouse Event

You can convert the mouse canvas position included in the event information to a world event if you have the camera
that the click occurred in and the SDL2 window of the game canvas.

There are three utility functions provided to calculate the world position:

- `MousePositionToCanvasPosition`
- `CanvasPositionToWorldPosition`
- `MousePositionToWorldPosition`

They exist in the same import as the `Camera` component.

```c++
#include "standard/sdl2_input/sdl2_input_system.hpp"
#include "standard/2d/camera/camera.hpp"
#include "standard/2d/transform/transform.hpp"
#include <SDL2/SDL.h>
...
void YourSystem::OnMessage(JamJar::Message *message) {
    MapSystem::OnMessage(message);
    switch (message->type) {
    case JamJar::Standard::SDL2InputSystem::MESSAGE_MOUSE_BUTTON_DOWN_EVENT: {
        auto *eventMessage = static_cast<JamJar::MessagePayload<JamJar::Standard::SDL2MouseEvent> *>(message);
        auto event = eventMessage->payload;

        if (this->entities.size() <= 0) {
            break;
        }

        // Get the first camera that is registered, assume that it is the one that is clicked (this holds true if
        // you only have one camera, if you have multiple more complex logic is required)
        auto cameraEntity = this->entities.begin()->second;
        auto transform = cameraEntity.Get<JamJar::Standard::_2D::Transform>();
        auto camera = cameraEntity.Get<JamJar::Standard::_2D::Camera>();

        // In this instance the SDL2_Window is stored in the YourSystem object as a member variable, allowing it to
        // be accessed. The SDL2_Window is not normally stored in a System and this needs to be set up by providing it
        // to the constructor.
        auto worldPos = JamJar::Standard::_2D::MousePositionToWorldPosition(event.position, transform->position, camera,
                                                                            this->window);

        std::cout << "listener got mouse down on world position x: " << worldPos.x << ", y:" << worldPos.y << std::endl;
        break;
    }
    }
}
```
