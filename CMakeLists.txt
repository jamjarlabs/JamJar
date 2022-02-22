cmake_minimum_required(VERSION 3.22)

set (CMAKE_CXX_STANDARD 17)

project(JamJar VERSION 0.0.0)

option(JAMJAR_NATIVE_COMPILE_UNIT_TESTS "Compile JamJar unit tests" ON)

### ===== dependencies ===== ###

include(FetchContent)

FetchContent_Declare(
    stb_image
    URL "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h"
    DOWNLOAD_NO_EXTRACT True
)

FetchContent_MakeAvailable(stb_image)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${stb_image_SOURCE_DIR})

set(BOX2D_BUILD_DOCS OFF CACHE BOOL "Build Box2D docs")
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "Build Box2D unit tests")
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "Build Box2D testbed")

FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/Box2D.git
    GIT_TAG v2.4.1
)

FetchContent_MakeAvailable(box2d)

if(JAMJAR_NATIVE_COMPILE_UNIT_TESTS)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.0-preview3
    )

    FetchContent_MakeAvailable(Catch2)
endif()

### ===== core ===== ###

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS  "-s USE_SDL=2")
else()
    # This is a bit of a hacky workaround, the CI runner is Ubuntu based, and the SDL2 library in Ubuntu doesn't play
    # nice with clang, so the SDL2 path needs to be manually included, see here:
    # https://bugs.launchpad.net/ubuntu/+source/libsdl2-ttf/+bug/1872023
    if(DEFINED ENV{CI})
        set(CMAKE_CXX_FLAGS "-I/usr/include/SDL2 -I${CMAKE_SOURCE_DIR}/build/_deps/stb_image-src -I${CMAKE_SOURCE_DIR}/build/_deps/box2d-src/include -I${CMAKE_SOURCE_DIR}/build/_deps/catch2-src/src -I${CMAKE_SOURCE_DIR}/include")
    endif()
endif()

set(LIB_SOURCES
    src/stb_image.cpp
    src/game.cpp
    src/window.cpp
    src/component/component_manager.cpp
    src/component/component.cpp
    src/entity/entity_manager.cpp
    src/entity/entity.cpp
    src/geometry/aabb.cpp
    src/geometry/matrix_4d.cpp
    src/geometry/polygon.cpp
    src/geometry/shape_2d.cpp
    src/geometry/vector_2d.cpp
    src/message/message_bus.cpp
    src/message/listener.cpp
    src/render/color.cpp
    src/render/material.cpp
    src/render/texture.cpp
    src/system/bucket_map_system.cpp
    src/system/bucket_system.cpp
    src/system/map_system.cpp
    src/system/single_entity_system.cpp
    src/system/stateful_system.cpp
    src/system/system_entity.cpp
    src/system/system.cpp
    src/system/vector_system.cpp
    src/standard/2d/box2d/box2d_body.cpp
    src/standard/2d/box2d/box2d_physics_system.cpp
    src/standard/2d/camera/camera.cpp
    src/standard/2d/interpolation/interpolation_system.cpp
    src/standard/2d/primitive/primitive_system.cpp
    src/standard/2d/primitive/primitive.cpp
    src/standard/2d/primitive/webgl2_default_primitive_shaders.cpp
    src/standard/2d/render/render_system.cpp
    src/standard/2d/sprite/sprite_system.cpp
    src/standard/2d/sprite/sprite.cpp
    src/standard/2d/sprite/webgl2_default_sprite_shaders.cpp
    src/standard/2d/transform/transform.cpp
    src/standard/2d/webgl2/webgl2_shader.cpp
    src/standard/2d/webgl2/webgl2_system.cpp
    src/standard/file_texture/file_texture_system.cpp
    src/standard/sdl2_input/sdl2_input_system.cpp
    src/standard/window/window_system.cpp
)

set(TEST_SOURCES
    src/geometry/vector_2d.test.cpp
)

add_library(JamJar
    ${LIB_SOURCES}
)

target_link_libraries(JamJar PRIVATE stb_image)
target_link_libraries(JamJar PRIVATE box2d)

target_include_directories(JamJar
    PUBLIC
        include
    PRIVATE
        src
)

if(JAMJAR_NATIVE_COMPILE_UNIT_TESTS)
    # Unit tests
    add_executable(
        JamJarTests
        ${TEST_SOURCES}
    )

    target_link_libraries(
        JamJarTests
        JamJar
        Catch2::Catch2WithMain
    )

    enable_testing()
    add_test(NAME JamJarTests COMMAND JamJarTests)

endif()

add_subdirectory (examples/Collision)
add_subdirectory (examples/Fullscreen)
add_subdirectory (examples/MouseInput)
add_subdirectory (examples/Physics)
add_subdirectory (examples/PhysicsResizing)
add_subdirectory (examples/Primitives)
add_subdirectory (examples/Rotator)
add_subdirectory (examples/Sprites)

### ===== clang-format ===== ###

# Find all source files
set(CLANG_FORMAT_CXX_FILE_EXTENSIONS ${CLANG_FORMAT_CXX_FILE_EXTENSIONS} *.cpp *.h *.cxx *.hxx *.hpp *.cc *.ipp)
file(GLOB_RECURSE ALL_SOURCE_FILES ${CLANG_FORMAT_CXX_FILE_EXTENSIONS})

# Don't include some common build folders
set(CLANG_FORMAT_EXCLUDE_PATTERNS "/build" "/external")

# get all project files file
foreach (SOURCE_FILE ${ALL_SOURCE_FILES})
    foreach (EXCLUDE_PATTERN ${CLANG_FORMAT_EXCLUDE_PATTERNS})
        string(FIND ${SOURCE_FILE} ${EXCLUDE_PATTERN} EXCLUDE_FOUND)
        if (NOT ${EXCLUDE_FOUND} EQUAL -1)
            list(REMOVE_ITEM ALL_SOURCE_FILES ${SOURCE_FILE})
        endif ()
    endforeach ()
endforeach ()

add_custom_target(beautify
    COMMENT "Running clang-format to beautify files"
    COMMAND clang-format
    -style=file
    -i
    ${ALL_SOURCE_FILES}
)

### ===== clang-tidy ===== ###

# Find all source files
set(CLANG_TIDY_CXX_FILE_EXTENSIONS ${CLANG_TIDY_CXX_FILE_EXTENSIONS} *.cpp *.h *.cxx *.hxx *.hpp *.cc *.ipp)
file(GLOB_RECURSE ALL_SOURCE_FILES ${CLANG_TIDY_CXX_FILE_EXTENSIONS})

# Don't include some common build folders
set(CLANG_TIDY_EXCLUDE_PATTERNS "/build" "/external" "/examples" "/include/hash.hpp")

# get all project files file
foreach (SOURCE_FILE ${ALL_SOURCE_FILES})
    foreach (EXCLUDE_PATTERN ${CLANG_TIDY_EXCLUDE_PATTERNS})
        string(FIND ${SOURCE_FILE} ${EXCLUDE_PATTERN} EXCLUDE_FOUND)
        if (NOT ${EXCLUDE_FOUND} EQUAL -1 OR "${SOURCE_FILE}" MATCHES "(.*)_test.cpp")
            list(REMOVE_ITEM ALL_SOURCE_FILES ${SOURCE_FILE})
        endif ()
    endforeach ()
endforeach ()

if(DEFINED ENV{CI})
    add_custom_target(lint
        COMMENT "Running clang-tidy to lint files"
        COMMAND clang-tidy
        ${ALL_SOURCE_FILES}
        -p build
        --
        -I/usr/include/SDL2
        -I${CMAKE_SOURCE_DIR}/build/_deps/stb_image-src
        -I${CMAKE_SOURCE_DIR}/build/_deps/box2d-src/include
        -I${CMAKE_SOURCE_DIR}/build/_deps/catch2-src/src
        -I${CMAKE_SOURCE_DIR}/include
        -std=c++17
    )
else()
    add_custom_target(lint
        COMMENT "Running clang-tidy to lint files"
        COMMAND clang-tidy
        ${ALL_SOURCE_FILES}
        -p build
    )
endif()

add_custom_target(lint_fix
    COMMENT "Running clang-tidy to lint files"
    COMMAND clang-tidy
    ${ALL_SOURCE_FILES}
    -p build
    -fix
    -fix-errors
)

### ===== docs ===== ###

add_custom_target(serve_docs
    COMMENT "Running docs server locally"
    COMMAND mkdocs serve -f ${CMAKE_SOURCE_DIR}/mkdocs.yml
)
