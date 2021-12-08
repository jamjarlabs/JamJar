#include "emscripten/html5.h"
#include "entity/entity_manager.hpp"
#include "fullscreen.hpp"
#include "game.hpp"
#include "input_listener.hpp"
#include "message/message_bus.hpp"
#include "message/message_payload.hpp"
#include "standard/2d/interpolation/interpolation_system.hpp"
#include "standard/2d/sprite/sprite_system.hpp"
#include "standard/2d/webgl2/webgl2_system.hpp"
#include "standard/file_texture/file_texture_system.hpp"
#include "standard/sdl2_input/sdl2_input_system.hpp"
#include "standard/window/window_system.hpp"
#include "window.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <string>

int main(int argc, char *argv[]) {

    auto window = JamJar::GetWindow("Fullscreen");
    auto context = JamJar::GetCanvasContext();

    auto messageBus = new JamJar::MessageBus();
    new JamJar::EntityManager(messageBus);
    auto game = new Fullscreen(messageBus);
    new JamJar::Standard::_2D::WebGL2System(messageBus, window, context);
    new JamJar::Standard::_2D::InterpolationSystem(messageBus);
    new JamJar::Standard::_2D::SpriteSystem(messageBus);
    new JamJar::Standard::FileTextureSystem(messageBus);
    new JamJar::Standard::SDL2InputSystem(messageBus);
    new JamJar::Standard::WindowSystem(messageBus, window, "canvas-wrapper");
    new InputListener(messageBus);
    game->Start();

    return 0;
}
