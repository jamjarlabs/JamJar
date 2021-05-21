cmake_minimum_required(VERSION 3.14)

set (CMAKE_CXX_STANDARD 17)

project(JamJarNative VERSION 0.0.0)

option(JAMJAR_NATIVE_COMPILE_UNIT_TESTS "Compile JamJarNative unit tests" OFF)

### ===== dependencies ===== ###

include(FetchContent)

if(JAMJAR_NATIVE_COMPILE_UNIT_TESTS)
    FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
    )

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
endif()

### ===== core ===== ###

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIB_SOURCES
    src/game.cpp
    src/component/component_manager.cpp
    src/component/component.cpp
    src/entity/entity_manager.cpp
    src/entity/entity.cpp
    src/geometry/matrix_4d.cpp
    src/geometry/vector_2d.cpp
    src/message/message_bus.cpp
    src/message/listener.cpp
    src/render/color.cpp
    src/system/map_system.cpp
    src/system/stateful_system.cpp
    src/system/system_entity.cpp
    src/system/system.cpp
    src/standard/2d/camera/camera.cpp
    src/standard/2d/interpolation/interpolation_system.cpp
    src/standard/2d/motion/motion_system.cpp
    src/standard/2d/motion/motion.cpp
    src/standard/2d/primitive/primitive.cpp
    src/standard/2d/primitive/primitive_system.cpp
    src/standard/2d/render/render_system.cpp
    src/standard/2d/transform/transform.cpp
    src/standard/2d/webgl2/webgl2_system.cpp
)

set(TEST_SOURCES
    src/game_test.cpp
)

add_library(JamJarNative
    ${LIB_SOURCES}
)

target_include_directories(JamJarNative
    PUBLIC
        include
    PRIVATE
        src
)

if(JAMJAR_NATIVE_COMPILE_UNIT_TESTS)
    # Unit tests
    add_executable(
        JamJarNativeTests
        ${TEST_SOURCES}
    )

    target_link_libraries(
        JamJarNativeTests
        JamJarNative
        gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(JamJarNativeTests)
endif()

add_subdirectory (examples/SimpleGame)

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

add_custom_target(lint
    COMMENT "Running clang-tidy to lint files"
    COMMAND clang-tidy
    ${ALL_SOURCE_FILES}
    -p build
)

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
