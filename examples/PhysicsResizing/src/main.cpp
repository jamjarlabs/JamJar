#include "emscripten/html5.h"
#include "entity/entity_manager.hpp"
#include "game.hpp"
#include "geometry/vector_2d.hpp"
#include "message/message_bus.hpp"
#include "message/message_payload.hpp"
#include "physics_resizing.hpp"
#include "resizing_system.hpp"
#include "standard/2d/box2d/box2d_physics_system.hpp"
#include "standard/2d/interpolation/interpolation_system.hpp"
#include "standard/2d/primitive/primitive_system.hpp"
#include "standard/2d/webgl2/webgl2_system.hpp"
#include "standard/file_texture/file_texture_system.hpp"
#include "standard/sdl2_input/sdl2_input_system.hpp"
#include "window.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <string>

int main(int argc, char *argv[]) {

    auto window = JamJar::GetWindow("PhysicsResizing");
    auto context = JamJar::GetCanvasContext();

    // Set up canvas
    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);
    auto context = emscripten_webgl_create_context("#canvas", &attrs);
    auto res = emscripten_webgl_make_context_current(context);

    auto messageBus = new JamJar::MessageBus();
    new JamJar::EntityManager(messageBus);
    auto game = new PhysicsResizing(messageBus);
    new JamJar::Standard::_2D::WebGL2System(messageBus, window, context);
    new JamJar::Standard::_2D::InterpolationSystem(messageBus);
    new JamJar::Standard::_2D::PrimitiveSystem(messageBus);
    new JamJar::Standard::_2D::Box2DPhysicsSystem(messageBus, JamJar::Vector2D(0, 0));
    new JamJar::Standard::SDL2InputSystem(messageBus);
    new ResizingSystem(messageBus);
    game->Start();

    return 0;
}
