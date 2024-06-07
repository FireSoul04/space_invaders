cmake_minimum_required(VERSION 3.5)
project(space_invaders)

# Create an option to switch between a system sdl library and a vendored sdl library
option(SPACE_INVADERS_VENDORED "Use vendored libraries" OFF)

if(SPACE_INVADERS_VENDORED)
    add_subdirectory(vendored/sdl EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
    find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

    # 1. Look for a SDL2 package, 2. Look for the SDL2maincomponent and 3. DO NOT fail when SDL2main is not available
    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
    find_package(SDL2_ttf REQUIRED CONFIG COMPONENTS SDL2_ttf)
endif()

set(FONT_PATH "${CMAKE_CURRENT_LIST_DIR}/assets/font.ttf")
configure_file(config.h.in "${CMAKE_CURRENT_LIST_DIR}/config.h")

# Create your game executable target as usual
add_executable(${PROJECT_NAME} #WIN32 
    space_invaders.cpp
    entities/entity.cpp
    entities/ship.cpp
    entities/alien.cpp
    entities/swarm.cpp
    entities/wall.cpp
    entities/projectile.cpp
    game_core/core.cpp
    game_core/entitylist.cpp
    game_core/keybuffer.cpp
)

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)
endif()

# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf::SDL2_ttf)