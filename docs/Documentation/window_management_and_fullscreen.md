# Window Management and Fullscreen

JamJar provides a system to manage the 'window', resizing the canvas, and dealing with full screen requests.

The `WindowSystem` manages the window.

## Setting up the Window System

To set up a `WindowSystem`, you need to provide a message bus, the SDL window to manage, and the ID of the 'wrapper'
HTML element that the canvas is contained in.

```c++
#include "window.hpp"
#include "standard/window/window_system.hpp"

...

auto window = JamJar::GetWindow("Fullscreen");

...

new JamJar::Standard::WindowSystem(messageBus, window, "canvas-wrapper");
```

This sets up the `WindowSystem` to manage the window, pointing to the canvas wrapper with the ID `canvas-wrapper`.

This constructor also can take in an initial aspect ratio value, if none is provided a default aspect ratio of `16/9`
is used.

```c++
// Using a 4/3 resolution
new JamJar::Standard::WindowSystem(messageBus, window, "canvas-wrapper", 4/3);
// Using a 4/3 resolution with a max resolution of 400x300
new JamJar::Standard::WindowSystem(messageBus, window, "canvas-wrapper", 4/3, 400, 300);
```

## Canvas Wrapper

The canvas wrapper is an HTML element that the canvas is in, this is the HTML element that is used for adjusting the
resolution of the canvas. The `WindowSystem` looks at the width and height of the canvas element as the maximum width
and height that it should scale the canvas to. This is to allow the site to integrate the game and its canvas into the
site easily, with scaling decisions made in HTML and CSS and stopping sites having to directly modify the canvas (which
can be tricky and mess up games easily).

For example:

```html
<style>
    #canvas-wrapper {
        display: flex;
        margin: auto;
        width: 100%;
        height: 100%;
        flex-direction: column;
        justify-content: center;
        flex-grow: 1;
    }

    #canvas {
        margin: auto;
        cursor: none;
        touch-action: none;
    }
</style>
<div id="canvas-wrapper">
    <canvas id="canvas"></canvas>
</div>
```

This is a typical set up, with the canvas wrapper configured to fit in to the site, with not much configuration required
of the canvas element itself.

## Resizing

The `WindowSystem` will listen out for any changes to the size of the browser window or full screen requests and
automatically resize using screen size, canvas size, aspect ratio, and maximum resolution.

The aspect ratio and maximum resolution can be adjusted by sending messages that the `WindowSystem` will listen for.

### Aspect Ratio

The aspect ratio is the aspect ratio that the canvas will adhere to, the `WindowSystem` will try and set the canvas
resolution to the highest possible that fits the aspect ratio provided.

The aspect ratio can be set at runtime with a message.

```c++
#include "standard/window/window_system.hpp"
#include "message/message_payload.hpp"

...

auto msg = std::make_unique<JamJar::MessagePayload<double>>(
    JamJar::Standard::WindowSystem::MESSAGE_SET_ASPECT_RATIO, 4/3);
messageBus->Publish(std::move(msg));
```

This sets the aspect ratio to `4/3`.

### Maximum Resolution

The maximum resolution is a hard limit on the maxmimum resolution the `WindowSystem` will allow, it defaults to
`x: -1, y: -1` with `-1` meaning no maximum is set.

An example of how the final resolution is calculated when using a maximum resolution:

- Screen size: `1920×1080`
- Aspect ratio: `4x3`
- Maximum resolution: `1000x1000`

Calculate the maximum size that fits in the maximum resolution: `x = 1000, y = 1000 * (1/(4/3)) = 750`.

Final resolution: `1000x750`

The maximum resolution can be set at runtime with a message.

```c++
#include "standard/window/window_system.hpp"
#include "message/message_payload.hpp"

...

auto msg = std::make_unique<JamJar::MessagePayload<std::pair<int, int>>>(
    JamJar::Standard::WindowSystem::MESSAGE_SET_MAX_RESOLUTION, std::pair<int, int>(400, 300));
messageBus->Publish(std::move(msg));
```

This sets the maximum resolution to `400x300`.

## Fullscreen

The `WindowSystem` uses emscripten's fullscreen functions to handle fullscreen interactions (entering and exiting).

### Entering Fullscreen

> Fullscreen is set up using emscripten's defer functionality, meaning if you request enter fullscreen without user
> input it will defer it until the user does something (e.g. clicks the canvas), then it will enter fullscreen. This
> is because of web security features. See the [emscripten documentation for more
> details](https://emscripten.org/docs/api_reference/html5.h.html#functions-affected-by-web-security).

You can enter fullscreen by sending a message:

```c++
#include "standard/window/window_system.hpp"
#include "message/message.hpp"

...

auto msg =
    std::make_unique<JamJar::Message>(JamJar::Standard::WindowSystem::MESSAGE_REQUEST_ENTER_FULLSCREEN);
this->messageBus->Publish(std::move(msg));
```

You can check if the request was successful by listening for these two events:

- `JamJar::Standard::WindowSystem::MESSAGE_ENTER_FULLSCREEN`
- `JamJar::Standard::WindowSystem::MESSAGE_ENTER_FULLSCREEN_FAILURE`

These messages are simple and do not contain any payload or additional data.

### Exiting Fullscreen

You can exit fullscreen by sending a message:

```c++
#include "standard/window/window_system.hpp"
#include "message/message.hpp"

...

auto msg =
    std::make_unique<JamJar::Message>(JamJar::Standard::WindowSystem::MESSAGE_REQUEST_EXIT_FULLSCREEN);
this->messageBus->Publish(std::move(msg));
```

You can check if the request was successful by listening for these two events:

- `JamJar::Standard::WindowSystem::MESSAGE_EXIT_FULLSCREEN`
- `JamJar::Standard::WindowSystem::MESSAGE_EXIT_FULLSCREEN_FAILURE`

These messages are simple and do not contain any payload or additional data.
