# Build configuration for the 2dp game, including SDL/SDL_image vendored libs and Box2D dependency setup.
cmake_minimum_required(VERSION 3.16)
project(2dp)
include(FetchContent)


# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")


# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)

# Box2D v3.1.0
FetchContent_Declare(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG        v3.1.0
        GIT_SHALLOW    TRUE
)
set(BOX2D_SAMPLES OFF CACHE BOOL "" FORCE)
set(BOX2D_UNIT_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(box2d)



# Create your game executable target as usual
add_executable(2dp WIN32 main.cpp
        Game.cpp
        Game.hpp
        TextureManager.h
        TextureManager.cpp
        GameObject.h
        GameObject.cpp
        map.h
        map.cpp
        ECS/ECS.h
        ECS/Component.h
        "ECS/SpriteComponent.h"
        ECS/PositionComponent.h)

# Link to the actual SDL3 and glm libraries.
target_link_libraries(2dp PRIVATE SDL3_image::SDL3_image SDL3::SDL3 box2d::box2d)
find_package(box2d CONFIG REQUIRED)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
