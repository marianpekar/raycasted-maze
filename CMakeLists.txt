cmake_minimum_required(VERSION 3.16)
project(raycasting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

find_package(SDL2 REQUIRED)

find_package(SDL2_image QUIET)
if(NOT SDL2_image_FOUND)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2_image REQUIRED IMPORTED_TARGET SDL2_image)
    add_library(SDL2_image::SDL2_image ALIAS PkgConfig::SDL2_image)
endif()

add_executable(raycasting
    source/Game.cpp
    source/main.cpp
    source/Maze.cpp
    source/Raycaster.cpp
    source/Renderer.cpp
    source/Surfaces.cpp
    source/Window.cpp
    source/PerlinNoise.cpp
    source/PathFinder.cpp
)

target_include_directories(raycasting PRIVATE source)

if(TARGET SDL2::SDL2)
    target_link_libraries(raycasting PRIVATE SDL2::SDL2)
else()
    target_include_directories(raycasting PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(raycasting PRIVATE ${SDL2_LIBRARIES})
endif()

target_link_libraries(raycasting PRIVATE SDL2_image::SDL2_image)

if(MSVC)
    target_compile_options(raycasting PRIVATE /W3)
else()
    target_compile_options(raycasting PRIVATE -Wall -Wextra)
endif()

add_custom_command(TARGET raycasting POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/source/images
            $<TARGET_FILE_DIR:raycasting>/images
    COMMENT "Copying images to output directory")