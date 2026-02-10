cmake_minimum_required(VERSION 3.16)
project(2dp)


# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")


# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)


# Create your game executable target as usual
add_executable(2dp WIN32 main.cpp
        Game.cpp
        Game.hpp
        TextureManager.h
        TextureManager.cpp
        GameObject.h
        GameObject.cpp
        map.h
        map.cpp)

# Link to the actual SDL3 and glm libraries.
target_link_libraries(2dp PRIVATE SDL3_image::SDL3_image SDL3::SDL3)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
