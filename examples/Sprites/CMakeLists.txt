cmake_minimum_required(VERSION 3.14)

project(Sprites VERSION 0.0.0)

### ===== dependencies ===== ###

# We don't need to compile the unit tests to use the library
SET(JAMJAR_COMPILE_UNIT_TESTS OFF CACHE BOOL "Compile JamJarNative unit tests")

include(FetchContent)

# This example relies directly on the latest code in the parent directory, if using the library externally instead
# this should be fetched from over the internet, e.g.:
# FetchContent_Declare(JamJarNative
#     GIT_REPOSITORY "git@github.com:jamjarlabs/JamJarNative.git"
#     GIT_TAG "<library version>"
# )

set(JAMJAR_NATIVE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../../")

FetchContent_Declare(JamJarNative
    SOURCE_DIR ${JAMJAR_NATIVE_SOURCE_DIR}
)

FetchContent_MakeAvailable(JamJarNative)

### ===== core ===== ###

set(CMAKE_CXX_FLAGS  "-s USE_SDL=2 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 --preload-file ${PROJECT_SOURCE_DIR}/assets@/assets")
SET(CMAKE_EXE_LINKER_FLAGS "-s LLD_REPORT_UNDEFINED")

set(EXE_SOURCES
    src/main.cpp
    src/simple_game.cpp
    src/input_listener.cpp
)

add_executable(Sprites ${EXE_SOURCES})

target_include_directories(Sprites
    PRIVATE
        src
)

target_link_libraries(Sprites PRIVATE JamJarNative)

configure_file(${PROJECT_SOURCE_DIR}/index.html ${PROJECT_BINARY_DIR}/index.html COPYONLY)
