#include "emscripten/html5.h"
#include "entity/entity_manager.hpp"
#include "game.hpp"
#include "message/message_bus.hpp"
#include "message/message_payload.hpp"
#include "simple_game.hpp"
#include "standard/2d/interpolation/interpolation_system.hpp"
#include "standard/2d/motion/motion_system.hpp"
#include "standard/2d/sprite/sprite_system.hpp"
#include "standard/2d/webgl2/webgl2_system.hpp"
#include "standard/file_texture/file_texture_system.hpp"
#include <iostream>
#include <memory>
#include <string>

int main(int argc, char *argv[]) {

    std::cout << "game start" << std::endl;

    // Set up canvas
    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);
    auto context = emscripten_webgl_create_context("#game-canvas", &attrs);
    auto res = emscripten_webgl_make_context_current(context);

    auto messageBus = new JamJar::MessageBus();
    new JamJar::EntityManager(messageBus);
    auto game = new SimpleGame(messageBus);
    new JamJar::Standard::_2D::WebGL2System(messageBus, context);
    new JamJar::Standard::_2D::MotionSystem(messageBus);
    new JamJar::Standard::_2D::InterpolationSystem(messageBus);
    new JamJar::Standard::_2D::SpriteSystem(messageBus);
    new JamJar::Standard::FileTextureSystem(messageBus);
    game->Start();

    return 0;
}
